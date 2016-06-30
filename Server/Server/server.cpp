#include "server.h"


server::server(int port, int maxC): MAX_CLIENTES(maxC)
{
	m_svRunning = true;
	m_alanTuring = new AlanTuring();
	printf("El puerto es %d \n",port);
	pthread_mutex_init(&m_serverInitMutex, NULL);
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condv, NULL);
	//Creo Socket
	sockfd =  socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("Error: No se pudo crear el socket");

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

	bzero((char *) &serv_addr, sizeof(serv_addr));

	/*Seteo la gilada basica del server*/
	serv_addr.sin_family = AF_INET;

	//Setea el adress del srv
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	//Transforma el int por el tema del endianness
	serv_addr.sin_port = htons(port);

	//Por ultimo enlazo el socket
	if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Server: No se enlazo correctamente el socket del server.");

	m_clientNum = 0;
	m_listaDeClientes.resize(MAX_CLIENTES);
	m_listaTimeOuts.resize(MAX_CLIENTES);
	m_clientThreads.resize(MAX_CLIENTES);
	m_queuePost.resize(MAX_CLIENTES);
	m_clientResponseThreads.resize(MAX_CLIENTES);
	startProcesarThread();

	printf("Bienvenido a servu\n");
}

server::~server()
{
	closeAllsockets();
	m_listaDeClientes.clear();
	m_listaTimeOuts.clear();
	m_queuePost.clear();

	delete m_alanTuring;
	pthread_mutex_destroy(&m_serverInitMutex);
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_condv);
}
void server::error(const char *msg)
{
	Logger::Instance()->LOG(msg, LogTypeError);
	Logger::Instance()->Close();
	exit(1);
}

void server::escuchar()
{
	int success = 0;
	success = listen(sockfd, MAX_CLIENTES);
	if (success < 0)
	{
		Logger::Instance()->LOG("Server: El server no se pudo configurar satisfactoriamente", LogTypeError);
		exit(-1);
	}
}
void server::aceptar(){
	//Aca el accept va a pisar el cli_addr y este nuevo es el sokete que lo relaciona a ese cliente
	//Deberia meter el nuevo thread por aca
	socklen_t clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
	{
		std::stringstream ss;
		ss <<"Server: No se pudo aceptar al cliente " << inet_ntoa(cli_addr.sin_addr) << ".";
		Logger::Instance()->LOG(ss.str(), LogTypeError);
		return;
	}
	if(getNumClientes() < MAX_CLIENTES)
	{
		//Mensaje connectedMessage = MessageFactory::Instance()->createMessage("", "", msgConnected);//Deprecated
		//sendMsg(newsockfd, connectedMessage);//Deprecated
		//Envio del mensaje connected dentro de crear Cliente
		bool playerCreated = crearCliente(newsockfd);
		if (!playerCreated)
		{
			std::stringstream ss;
			ss <<"Server: No se pudo aceptar al cliente " << inet_ntoa(cli_addr.sin_addr) << " por nombre inválido.";
			Logger::Instance()->LOG(ss.str(), LogTypeWarn);
		}
	}
	else
	{
		std::stringstream ss;
		ss <<"Server: No se pudo aceptar al cliente " << inet_ntoa(cli_addr.sin_addr) << " por falta de capacidad.";
		Logger::Instance()->LOG(ss.str(), LogTypeWarn);

		Mensaje serverFullMessage = MessageFactory::Instance()->createMessage("svfull", "", msgServerFull);
		sendMsg(newsockfd, serverFullMessage);
		close(newsockfd);
		return;
	}


}

bool server::crearCliente (int clientSocket)
{
	pthread_mutex_lock(&m_serverInitMutex);
	//m_lastID almacena el indice de la lista Inteligente en el que el cliente fue agregado
	m_lastID = m_listaDeClientes.add(clientSocket);

	//variable de control
	m_successfulPlayerCreation = false;

	if (m_lastID < 0)
	{
		Logger::Instance()->LOG("Server: Cliente rechazado. El servidor no puede aceptar más clientes.", LogTypeWarn);
		pthread_mutex_unlock(&m_serverInitMutex);
		return false;
	}

	agregarTimeOutTimer(m_lastID);

	//Envia solicitud de datos de coneccion
	ConnectedMessage connectedMsg;
	connectedMsg.requestData = true;
	connectedMsg.connected = false;
	connectedMsg.objectID = m_lastID;
	connectedMsg.textureID = m_lastID;
	//Tamaño de ventana
	connectedMsg.windowWidth = Game::Instance()->getGameWidth();
	connectedMsg.windowHeight = Game::Instance()->getGameHeight();

	//Info de modo de juego
	connectedMsg.teamMode = Game::Instance()->isTeamMode();

	if (connectedMsg.teamMode == true) {
		std::vector<GameTeam> teamList = Game::Instance()->gameTeams();
		std::stringstream teamStringStream;
		std::transform(teamList.begin(),
				teamList.end(),
				std::ostream_iterator<std::string>(teamStringStream, "|"),
				[](const GameTeam &team) {
			return team.gameTeamName + "-" + std::to_string(team.gameTeamID);
		});

		std::string teamsName = teamStringStream.str();
		std::size_t teamsNameLenght = teamsName.copy(connectedMsg.teamsName, 128, 0);
		connectedMsg.teamsName[teamsNameLenght - 1]='\0';
    } else {
        connectedMsg.teamsName[0] = '\0';
    }

	connectedMsg.cantPlayers = MAX_CLIENTES;
	sendConnectedMsg(clientSocket, connectedMsg);

	if (!leerBloqueando(m_lastID))
	{
		//No se pudo crear el jugador
		removeTimeOutTimer(m_lastID);
		m_listaDeClientes.removeAt(m_lastID);
		close(clientSocket);
		pthread_mutex_unlock(&m_serverInitMutex);
		return false;
	}

	//Informa resultado del procesamiento de la informacion de coneccion
	connectedMsg.requestData = false;
	connectedMsg.connected = m_successfulPlayerCreation;

	if (!m_successfulPlayerCreation)
	{
		sendConnectedMsg(clientSocket, connectedMsg);
		removeTimeOutTimer(m_lastID);
		m_listaDeClientes.removeAt(m_lastID);
		close(clientSocket);
		pthread_mutex_unlock(&m_serverInitMutex);
		return false;
	}

	aumentarNumeroClientes();

	pthread_create(&m_clientThreads[m_lastID], NULL, &server::mati_method, (void*)this);
	pthread_create(&m_clientResponseThreads[m_lastID], NULL, &server::mati_method3, (void*)this);

	sendConnectedMsg(clientSocket, connectedMsg);

	printf("Se ha conectado un cliente\n");
	std::stringstream ss;
	ss << "Server: Se acepto el cliente: " << inet_ntoa(cli_addr.sin_addr);
	Logger::Instance()->LOG(ss.str(), LogTypeDebug);
	pthread_mutex_unlock(&m_serverInitMutex);

	return true;
}

void server::agregarTimeOutTimer(int clientPosition)
{
	//comienza el timer
	Timer timer;
	m_listaTimeOuts.addAt(clientPosition, timer);
	m_listaTimeOuts.getElemAt(clientPosition).Reset();
	m_listaTimeOuts.getElemAt(clientPosition).Start();
}
void server::removeTimeOutTimer(int clientPosition)
{
	m_listaTimeOuts.getElemAt(clientPosition).Reset();
	m_listaTimeOuts.getElemAt(clientPosition).Stop();
	m_listaTimeOuts.removeAt(clientPosition);
}

void server::escribir(int id)
{

	//send(m_listaDeClientes.getElemAt(id), "Llego correctamente!\n", 21, 0);
}

void server::encolarDrawMessage(DrawMessage drawMsg)
{
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			NetworkMessage netMsg = m_alanTuring->drawMessageToNetwork(drawMsg);
			m_queuePost[i].add(netMsg);
		}
	}

}

void server::sendDrawMsgToAll(DrawMessage drawMsg){

	NetworkMessage netMsg = m_alanTuring->drawMessageToNetwork(drawMsg);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}

void server::sendPlayerNames(PlayerNameUpdateInfo playerNameInfo)
{
	NetworkMessage netMsg = m_alanTuring->playerNameUpdateInfoToNetwork(playerNameInfo);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}

void server::sendResetMsgToAll(ResetInfo resetMsg)
{

	NetworkMessage netMsg = m_alanTuring->ResetMsgToNetwork(resetMsg);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}

void server::sendScoreMsgToAll(ScoreMessage scoreMsg)
{

	NetworkMessage netMsg = m_alanTuring->ScoreMessageToNetwork(scoreMsg);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}
void server::sendBackgroundInfoToAll(BackgroundInfo backgroundInfo)
{

	NetworkMessage netMsg = m_alanTuring->BackgroundInfoToNetwork(backgroundInfo);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}
void server::sendStageStatistics(StageStatistics stageStatistics, int clientID)
{

	NetworkMessage netMsg = m_alanTuring->StageStatisticsToNetwork(stageStatistics);
	if ( m_listaDeClientes.isAvailable(clientID))
	{
		m_queuePost[clientID].add(netMsg);
	}
}

void server::sendPlayerDataToClient(PlayerDataUpdateInfo playerDataInfo)
{
	int clientID = playerDataInfo.playerID;
	NetworkMessage netMsg = m_alanTuring->playerDataUpdateInfoToNetwork(playerDataInfo);
	if ( m_listaDeClientes.isAvailable(clientID))
	{
		m_queuePost[clientID].add(netMsg);
	}
}

void server::sendStageBeginningInfoToAll(StageBeginning stageBeginningInfo)
{

	NetworkMessage netMsg = m_alanTuring->StageBeginningToNetwork(stageBeginningInfo);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}

void server::sendFinishGameInfoToAll(FinishGameInfo finishGameInfo)
{

	NetworkMessage netMsg = m_alanTuring->FinishGameInfoToNetwork(finishGameInfo);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}

void sendStageBeginningInfoToAll(StageBeginning stageBeginningInfo);
void sendFinishGameInfoToAll(FinishGameInfo finishGameInfo);

void server::informTextureInfos(int clientID)
{
	ParserNivel* parserNivel = new ParserNivel();
	TextureHelper* textureHelper = new TextureHelper();

	parserNivel->parsearDocumento(XML_PATH);
	std::vector<Sprite> sprites = parserNivel->getListaSprites();
	for (std::vector<Sprite>::iterator it = sprites.begin() ; it !=  sprites.end(); ++it)
	{
		TextureInfo textureInfoToSend;
		//id string to int
		textureInfoToSend.textureID = textureHelper->stringToInt((*it).id);
		//path string to buffer
		std::size_t length = (*it).path.copy(textureInfoToSend.path, PATH_MAX_LENGTH, 0);
		textureInfoToSend.path[length]='\0';
		//otras variables de imagen
		textureInfoToSend.numFrames = (*it).cantidad;
		textureInfoToSend.height = (*it).alto;
		textureInfoToSend.width = (*it).ancho;
		textureInfoToSend.lastTexture = false;

		if (boost::next(it) == sprites.end())
		{
			textureInfoToSend.lastTexture = true;
		}

		NetworkMessage networkMessage = m_alanTuring->TextureInfoToNetwork(textureInfoToSend);
		m_queuePost[clientID].add(networkMessage);
	}
	delete parserNivel;
	delete textureHelper;
}

void server::sendPackToAll(DrawMessagePack drawPackMsg){
	NetworkMessage netMsg = m_alanTuring->drawMsgPackToNetwork(drawPackMsg);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{
			m_queuePost[i].add(netMsg);
		}
	}
}

void server::informPlayerDisconnection(PlayerDisconnection playerDiscMsg, int playerDiscID){

	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if (i == playerDiscID)
			continue;
		if ( m_listaDeClientes.isAvailable(i))
		{
			NetworkMessage netMsg = m_alanTuring->playerDisconnectionToNetwork(playerDiscMsg);
			m_queuePost[i].add(netMsg);
		}
	}
}


void server::informGameBeginning()
{
	NetworkMessage gameBeginningMsg;
	gameBeginningMsg.msg_Code[0] = 'g';
	gameBeginningMsg.msg_Code[1] = 'b';
	gameBeginningMsg.msg_Code[2] = 'g';
	gameBeginningMsg.msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if ( m_listaDeClientes.isAvailable(i))
		{

			m_queuePost[i].add(gameBeginningMsg);
		}
	}
}

void server::informPlayerReconnected(int clientID)
{
	PlayerReconnectionInfo playerRecInfo;
	playerRecInfo.playerID = clientID;
	NetworkMessage netWorkMessage = m_alanTuring->PlayerReconnectionInfoToNetwork(playerRecInfo);
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		if (i == clientID)
			continue;
		if ( m_listaDeClientes.isAvailable(i))
		{

			m_queuePost[i].add(netWorkMessage);
		}
	}
}

void server::informGameBegan(int clientID)
{
	NetworkMessage gameBeginningMsg;
	gameBeginningMsg.msg_Code[0] = 'g';
	gameBeginningMsg.msg_Code[1] = 'b';
	gameBeginningMsg.msg_Code[2] = 'g';
	gameBeginningMsg.msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;
	m_queuePost[clientID].add(gameBeginningMsg); //Hace falta el isAvailable?
}



void server::sendDrawMsg(int socketReceptor, DrawMessage msg)
{
	char bufferEscritura[MESSAGE_BUFFER_SIZE];
	bzero(bufferEscritura,MESSAGE_BUFFER_SIZE);

	int msgLength = m_alanTuring->encodeDrawMessage(msg, bufferEscritura);

	char *ptr = (char*) bufferEscritura;

	while (msgLength > 0)
	{
		int bytesEnviados = send(socketReceptor, ptr, msgLength, 0);
		if (bytesEnviados < 1)
		{
			Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);
			return;
		}
		ptr += bytesEnviados;
		msgLength -= bytesEnviados;
	}
}

void server::sendNetworkMsg(int socketReceptor, NetworkMessage netMsg)
{
	char bufferEscritura[MESSAGE_BUFFER_SIZE];
	bzero(bufferEscritura,MESSAGE_BUFFER_SIZE);

	int msgLength = m_alanTuring->encodeNetworkMessage(netMsg, bufferEscritura);

	char *ptr = (char*) bufferEscritura;

	while (msgLength > 0)
	{
		int bytesEnviados = send(socketReceptor, ptr, msgLength, 0);
		if (bytesEnviados < 1)
		{
			Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);
			return;
		}
		ptr += bytesEnviados;
		msgLength -= bytesEnviados;
	}
}

void server::sendConnectedMsg(int socketReceptor, ConnectedMessage msg)
{
	char bufferEscritura[MESSAGE_BUFFER_SIZE];
	int msgLength = m_alanTuring->encodeConnectedMessage(msg, bufferEscritura);
	char *ptr = (char*) bufferEscritura;

	while (msgLength > 0)
	{
		int bytesEnviados = send(socketReceptor, ptr, msgLength , 0);
		if (bytesEnviados < 1)
		{
			Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);
			return;
		}
		ptr += bytesEnviados;
		msgLength -= bytesEnviados;
	}
}
void server::sendDisconnectionMsg(int socketReceptor, PlayerDisconnection msg)
{
	char bufferEscritura[MESSAGE_BUFFER_SIZE];
	int msgLength = m_alanTuring->encodePlayerDisconnectionMessage(msg, bufferEscritura);
	char *ptr = (char*) bufferEscritura;

	while (msgLength > 0)
	{
		int bytesEnviados = send(socketReceptor, ptr, msgLength , 0);
		if (bytesEnviados < 1)
		{
			Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);
			return;
		}
		ptr += bytesEnviados;
		msgLength -= bytesEnviados;
	}
}

void server::sendMsg(int socketReceptor, Mensaje msg)
{
	char bufferEscritura[MESSAGE_BUFFER_SIZE];
	int msgLength = m_alanTuring->encodeXMLMessage(msg, bufferEscritura);
	char *ptr = (char*) bufferEscritura;

	while (msgLength > 0)
	{
		int bytesEnviados = send(socketReceptor, ptr, msgLength, 0);
		if (bytesEnviados < 1)
		{
			Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);
			return;
		}
		ptr += bytesEnviados;
		msgLength -= bytesEnviados;
	}

	/*int bytesEnviados = send(socketReceptor,bufferEscritura , msgLength, 0);
    if (bytesEnviados <= 0)
    	Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);
    while (bytesEnviados < msgLength)
    {
    	bytesEnviados =  send(sockfd, bufferEscritura + bytesEnviados , msgLength, 0);
        if (bytesEnviados <= 0)
        	Logger::Instance()->LOG("Server: No se pudo enviar el mensaje.", LogTypeWarn);

    }*/
}

bool server::leer(int id)
{
	//Reseteo el buffer que se va a completar con nuevos mensajes
	bzero(buffer, MESSAGE_BUFFER_SIZE);
	char *p = (char*)buffer;
	int messageLength = 0;

	int readLimit = (DRAW_MESSAGE_PACK_SIZE * sizeof(DrawMessage)) + MESSAGE_CODE_BYTES + MESSAGE_LENGTH_BYTES + MESSAGE_LENGTH_BYTES;

	int n = recv(m_listaDeClientes.getElemAt(id), buffer, MESSAGE_LENGTH_BYTES, 0);

	if (!lecturaExitosa(n, id))
		return false;

	int acum = n;
	// bool bug = false;
	while (acum < MESSAGE_LENGTH_BYTES)
	{
		p += n;
		n = recv(m_listaDeClientes.getElemAt(id), p, MESSAGE_LENGTH_BYTES - acum, 0);
		if (!lecturaExitosa(n, id))
			return false;
		acum += n;

	}
	messageLength = m_alanTuring->decodeLength(buffer);
	//printf("Longitud Mensaje: %d \n", messageLength);

	p += n;
	messageLength -= acum;

	//loopea hasta haber leido la totalidad de los bytes necarios
	while (messageLength > 0)
	{
		//printf("Faltan leer %d \n", messageLength);
		n = recv(m_listaDeClientes.getElemAt(id), p, messageLength, 0);

		if(messageLength >readLimit)
		{
			return true;
		}

		if (!lecturaExitosa(n, id))
			return false;
		p += n;
		messageLength -= n;
	}

	//resetea el timer de timeout
	m_listaTimeOuts.getElemAt(id).Reset();

	NetworkMessage netMsgRecibido = m_alanTuring->decode(buffer);

	ServerMessage serverMsg;
	serverMsg.clientID = id;
	serverMsg.networkMessage = netMsgRecibido;
	/*string my_str2 (buffer);
    mensije msg;
    msg.id = id;
    msg.texto = my_str2;*/

	m_mensajesAProcesar.add(serverMsg);
	return true;
}

void* server::procesar(void)
{
	while(this->isRunning())
	{

		//Chekea timeouts
		checkTimeOuts();

		//Procesa cola
		if (m_mensajesAProcesar.size() != 0)
		{
			ServerMessage serverMsg = m_mensajesAProcesar.remove();
			// BLOQUE DE PROCESAMIENTO
			procesarMensaje(&serverMsg);


			// BLOQUE DE PROCESAMIENTO
			//m_queuePost[serverMsg.clientID].add(serverMsg);

		}

	}
	return NULL;
}

void server::checkTimeOuts()
{
	for (int i = 0; i < m_listaTimeOuts.size(); ++i)
	{
		if ((!m_listaTimeOuts.isAvailable(i)) || (!m_listaDeClientes.isAvailable(i)))
		{
			continue;
		}
		//printf("Timer del server = %f\n", (float)m_listaTimeOuts.getElemAt(i).GetTicks()/CLOCKS_PER_SEC);
		if ((long double)(m_listaTimeOuts.getElemAt(i).GetTicks()/CLOCKS_PER_SEC) >= TIMEOUT_SECONDS)
		{
			//printf("Timer del cliente %d = %f\n", i, (float)m_listaTimeOuts.getElemAt(i)->GetTicks()/CLOCKS_PER_SEC);
			//printf("El cliente con id %d timeouteo.\n", i);
			//printf("Close Timeout\n");
			closeSocket(i);
		}
	}
}

void* server::postProcesamiento(void)
{
	int id = this->m_lastID;

	bool ciclar = true;
	while(ciclar)
	{
		if (m_queuePost[id].size() != 0)
		{
			NetworkMessage netMsg = m_queuePost[id].remove();
			//DrawMessage drawMessage =  m_alanTuring->decodeDrawMessage(netMsg);

			sendNetworkMsg(m_listaDeClientes.getElemAt(id),netMsg);
		}
	}
	return NULL;
}


void *server::newDialog(void)
{
	int id = this->m_lastID;
	while(true)
	{
		if (!this->leer(id))
			break;
	}
	pthread_exit(NULL);

}
// nunca deberais desafiar el poder del matimethod...o sera muy tarde...fuera de joda no tokes nada aca gil ah si? ven aqui
void *server::mati_method(void *context)
{
	return ((server *)context)->newDialog();
}
void *server::mati_method2(void *context)
{
	return ((server *)context)->procesar();
}
void *server::mati_method3(void *context)
{
	return ((server *)context)->postProcesamiento();
}


void server::startProcesarThread()
{
	pthread_create(&threadDeProcesar, NULL, mati_method2,(void*)this );
}

void server::closeAllsockets()
{
	//Cuando el Server tenga lista de threads hay q recorrer cerrando
	for (int i = 0; i < m_listaDeClientes.size(); i++)
	{
		closeSocket(i);
	}
	//close(newsockfd);
	close(sockfd);
}

void server::closeSocket(int id)
{
	if (!m_listaDeClientes.isAvailable(id))
		return;

	removeTimeOutTimer(id);

	Game::Instance()->disconnectPlayer(id);

	reducirNumeroClientes();
	close(m_listaDeClientes.getElemAt(id));
	m_listaDeClientes.removeAt(id);

	Logger::Instance()->LOG("Server: Se desconectó un cliente.", LogTypeDebug);
	printf("Se desconectó un cliente, hay lugar para %d clientes mas.\n",MAX_CLIENTES - getNumClientes());

}

void server::aumentarNumeroClientes()
{
	pthread_mutex_lock(&m_mutex);
	if (m_clientNum < MAX_CLIENTES)
		m_clientNum++;
	pthread_cond_signal(&m_condv);
	pthread_mutex_unlock(&m_mutex);
}
void server::reducirNumeroClientes()
{
	pthread_mutex_lock(&m_mutex);
	if (m_clientNum > 0)
		m_clientNum--;
	pthread_cond_signal(&m_condv);
	pthread_mutex_unlock(&m_mutex);
}
const int server::getNumClientes()
{
	pthread_mutex_lock(&m_mutex);
	int cant = m_clientNum;
	pthread_mutex_unlock(&m_mutex);
	return cant;
}

const int server::getMaxClientes()
{
	return MAX_CLIENTES;
}
bool server::isRunning()
{
	return m_svRunning;
}

/*void server::setTimeOut(int socketID)
{
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = TIMEOUT_MICROSECONDS;

    if (setsockopt (socketID, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
    	Logger::Instance()->LOG("Server: Error seteando timeout.", LogTypeWarn);
    }

    if (setsockopt (socketID, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
    	Logger::Instance()->LOG("Server: Error seteando timeout.", LogTypeWarn);
    }
}*/

bool server::lecturaExitosa(int bytesLeidos, int clientID)
{
	if (bytesLeidos < 0)
	{
		//Cliente Desconectado
		//printf ("Close lectura -1\n");
		closeSocket(clientID);
		return false;

	}
	if (bytesLeidos == 0)
	{
		//Cliente Desconectado. Hay diferencias con recibir -1? Sino lo ponemos to do junto, hacen lo mismo
		//printf ("Close lectura 0\n");
		closeSocket(clientID);
		return false;
	}
	return true;
}

bool server::procesarMensaje(ServerMessage* serverMsg)
{

	NetworkMessage netMsg = serverMsg->networkMessage;

	//Time Out Msg
	if ((netMsg.msg_Code[0] == 't') && (netMsg.msg_Code[1] == 'm') && (netMsg.msg_Code[2] == 'o'))
	{
		NetworkMessage timeOutMsg;
		timeOutMsg.msg_Code[0] = 't';
		timeOutMsg.msg_Code[1] = 'm';
		timeOutMsg.msg_Code[2] = 'o';
		timeOutMsg.msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

		//agrega solo en el cliente del cual recibio el mensaje
		m_queuePost[serverMsg->clientID].add(timeOutMsg);
		return true;
	}

	//Connection Info Message
	if ((netMsg.msg_Code[0] == 'c') && (netMsg.msg_Code[1] == 'n') && (netMsg.msg_Code[2] == 'i'))
	{
		ConnectionInfo connectionInfoMessage = m_alanTuring->decodeConnectionInfoMessage(netMsg);
		std::string playerName = std::string(connectionInfoMessage.playerName);
        printf("Se conecto %s \n", connectionInfoMessage.playerName);
        int playerTeam = connectionInfoMessage.playerTeamID;
		m_successfulPlayerCreation = Game::Instance()->createPlayer(m_lastID, playerName, playerTeam);

		if (m_successfulPlayerCreation)
		{
			std::stringstream ss;
			ss <<"Server: " << playerName << " ha ingresado a la partida.";
			Logger::Instance()->LOG(ss.str(), LogTypeDebug);
			printf("%s \n", ss.str().c_str());
		}

		return m_successfulPlayerCreation;
	}

	//Reset Msg
	if ((netMsg.msg_Code[0] == 'r') && (netMsg.msg_Code[1] == 's') && (netMsg.msg_Code[2] == 't'))
	{
		if (!Game::Instance()->isResseting())
		{
			Game::Instance()->setReseting(true);
			Logger::Instance()->LOG("Server: Se reiniciará el juego.", LogTypeDebug);
			//Resetea el juego
			Game::Instance()->restartLevel();
			//Envia la nueva informacion al cliente
			ResetInfo resetInfo;
			resetInfo.windowHeight = Game::Instance()->getGameHeight();
			resetInfo.windowWidth = Game::Instance()->getGameWidth();

			sendResetMsgToAll(resetInfo);

			Game::Instance()->refreshPlayersDirty();
			Game::Instance()->setReseting(false);
			Logger::Instance()->LOG("Server: Se ha reiniciado el juego.", LogTypeDebug);
		}

		return true;
	}

	//TextureInfo Request
	if ((netMsg.msg_Code[0] == 't') && (netMsg.msg_Code[1] == 'x') && (netMsg.msg_Code[2] == 'r'))
	{
		informTextureInfos(serverMsg->clientID);
		return true;
	}


	//Input Msg
	if ((netMsg.msg_Code[0] == 'i') && (netMsg.msg_Code[1] == 'm') && (netMsg.msg_Code[2] == 's'))
	{
		//printf("Decodificando Input Msg\n");
		InputMessage inputMsg = m_alanTuring->decodeInputMessage(netMsg);
		Game::Instance()->actualizarEstado(serverMsg->clientID,inputMsg);
		//printf(" Fin Decodificando Input Msg\n");
		return true;
	}

	return true;
}

bool server::leerBloqueando(int id)
{
	//Reseteo el buffer que se va a completar con nuevos mensajes
	bzero(buffer, MESSAGE_BUFFER_SIZE);
	char *p = (char*)buffer;
	int messageLength = 0;

	int n = recv(m_listaDeClientes.getElemAt(id), p, MESSAGE_LENGTH_BYTES, 0);
	if (!lecturaExitosa(n, id))
		return false;

	int acum = n;
	while (n < 4)
	{
		p += n;
		n = recv(m_listaDeClientes.getElemAt(id), p, MESSAGE_LENGTH_BYTES, 0);
		if (!lecturaExitosa(n, id))
			return false;
		acum += n;
	}
	messageLength = m_alanTuring->decodeLength(buffer);

	p += n;
	messageLength -= acum;

	//loopea hasta haber leido la totalidad de los bytes necarios
	while (messageLength > 0)
	{
		n = recv(m_listaDeClientes.getElemAt(id), p, messageLength, 0);
		if (!lecturaExitosa(n, id))
			return false;
		p += n;
		messageLength -= n;
	}

	//resetea el timer de timeout
	m_listaTimeOuts.getElemAt(id).Reset();

	NetworkMessage netMsgRecibido = m_alanTuring->decode(buffer);

	ServerMessage serverMsg;
	serverMsg.clientID = id;
	serverMsg.networkMessage = netMsgRecibido;
	/*string my_str2 (buffer);
    mensije msg;
    msg.id = id;
    msg.texto = my_str2;*/

	procesarMensaje(&serverMsg);
	return true;
}

