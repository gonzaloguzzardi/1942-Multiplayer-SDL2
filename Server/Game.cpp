#include "Game.h"

#include "Enemies/SmallEnemy.h"
#include "Enemies/BossMati.h"
#include "Enemies/BigPlane.h"
#include "Enemies/MediumPlane.h"
#include "Enemies/Formation.h"
#include "PowerUps/ExtraPointsPU.h"
#include "PowerUps/BombPU.h"
#include "PopUps/PopUp.h"
#include "Player.h"
#include "Weapons/BulletsHandler.h"
#include "Singletons/CollisionHandler.h"
#include "Spawners/PowerUpSpawner.h"
#include "Spawners/EnemySpawner.h"
#include <sstream>

Game* Game::s_pInstance = 0;

Game::Game():
m_pWindow(0),
m_pRenderer(0),
m_stagesAmount(1),
m_currentStage(1),
m_practiceMode(false),
m_practiceHoldTimer(0),
m_startingWaitTime(500),
m_waitEndStageTimer(END_STAGE_TIMER),
m_gameOver(false),
m_running(false),
m_reseting(false),
m_startingStage(true),
m_endingStage(false),
m_scrollingToNextStage(false),
m_waitingToScroll(false),
m_stageStarted(false),
m_scrollBackground(false),
m_scrollSpeed(2)
{
	m_powerUpsSpawner = new PowerUpSpawner();
	m_enemiesSpawner = new EnemySpawner();

	/*GameTeam teamOne;
	teamOne.gameTeamID = 0;
	teamOne.gameTeamScore = 0;
	teamOne.gameTeamName = "team 1";
	m_teamsList.push_back(teamOne);
	GameTeam teamTwo;
	teamOne.gameTeamID = 1;
	teamOne.gameTeamScore = 0;
	teamOne.gameTeamName = "team 2";
	m_teamsList.push_back(teamTwo);*/

	pthread_mutex_init(&m_resetMutex, NULL);
	pthread_mutex_init(&m_updatePlayerMutex, NULL);
	pthread_mutex_init(&m_createPlayerMutex, NULL);
}

Game::~Game()
{
    // we must clean up after ourselves to prevent memory leaks
    m_pRenderer= 0;
    m_pWindow = 0;
    pthread_mutex_destroy(&m_resetMutex);
    pthread_mutex_destroy(&m_updatePlayerMutex);
    pthread_mutex_destroy(&m_createPlayerMutex);
}


bool Game::init(const char* title, int xpos, int ypos, int width, int height)
{
    // Tamaño de la ventana

    m_parserNivel = new ParserNivel();
    m_parserNivel->parsearDocumento(XML_PATH);
    m_stagesAmount = m_parserNivel->getEscenario().cantidadStages;
    for (int i = 0; i < m_parserNivel->getEscenario().teamsList.size();i++)
    {
    	GameTeam teamInfo;
    	teamInfo.gameTeamID = m_parserNivel->getEscenario().teamsList[i].gameTeamID;
    	teamInfo.gameTeamScore = 0;
    	teamInfo.gameTeamName = m_parserNivel->getEscenario().teamsList[i].gameTeamName;
    	m_teamsList.push_back(teamInfo);
    }

    m_textureHelper = new TextureHelper();

    m_gameWidth = m_parserNivel->getVentana().ancho;
    m_gameHeight = m_parserNivel->getVentana().alto;

    //es necesario llamar a initialize Textures despues de haber creado el parser y textureHelper porque los usa
    initializeTexturesInfo();

    //printf("Path isla: %s \n", m_parserNivel->getListaSprites()[5].path.c_str());
   // printf("ID isla: %s \n", m_parserNivel->getListaSprites()[5].id.c_str());

    printf("Se cargo el escenario con ancho %d y alto %d\n", m_gameWidth, m_gameHeight);

    inicializarServer();

    m_level = new Level();
    m_level->loadFromXML(XML_PATH);

   // enemy = new BossMati();
    //enemy->load(m_gameWidth/2,0,48,48,666,4);
    //CollitionHandler::Instance()->addEnemy(enemy);

    sendPlayerNames();

    loadCurrentStage();
    startStage();

    //tudo ben
    m_running = true;

    return true;
}

bool Game::createPlayer(int clientID,  const std::string& playerName, int playerTeam)
{
	pthread_mutex_lock(&m_createPlayerMutex);

	bool nameExists;
	std::stringstream ss;

	//Se fija si existe un jugador con el nombre ingresado
	nameExists = !validatePlayerName(playerName);
    
    // Setea el equipo.
    /*GameTeam selectedTeam;
    std::vector<GameTeam> teamsList = this->m_parserNivel->getEscenario().teamsList;
    std::vector<GameTeam>::iterator iterator = teamsList.begin();
    for (*iterator; iterator != teamsList.end(); ++iterator) {
        GameTeam newGameTeam = *iterator;
        
        if (newGameTeam.gameTeamID == playerTeam) {
            selectedTeam = newGameTeam;
        	m_teamsList[newGameTeam.gameTeamID].gameTeamName = newGameTeam.gameTeamName;
        }
    }*/

	if (nameExists)
	{
		int actualPlayerID = getFromNameID(playerName);
		Player* player = m_listOfPlayer[actualPlayerID];
        
        player->setPlayerTeam(m_teamsList[playerTeam]);

		//agrega jugador al manejador de colisiones
		CollitionHandler::Instance()->addPlayer(player);

		if (player->isConnected()) //El jugador con ese nombre ya esta conectado
		{
			ss <<"Server: El jugador con nombre" << playerName << " ya se encuentra conectado.";
			Logger::Instance()->LOG(ss.str(), LogTypeWarn);
			printf("%s \n", ss.str().c_str());
			player->refreshDirty();
			pthread_mutex_unlock(&m_createPlayerMutex);
			return false;
		}
		else //Se desconecto y se esta volviendo a conectar
		{
			player->setConnected(true);
			player->setPlayerTeam(m_teamsList[playerTeam]);
			player->refreshDirty();
			m_server->informGameBegan(clientID);
			m_server->informPlayerReconnected(clientID);
			setPlayersDirty();

			pthread_mutex_unlock(&m_createPlayerMutex);
			return true;
		}
	}
	//Si no existe el nombre:

	//Esto controla que solo se puedan volver a conectar los que arrancaron jugando al ppio

	if (m_listOfPlayer.size() == m_parserNivel->getEscenario().cantidadJugadores)
	{
		ss <<"Server: El jugador con nombre" << playerName << " no se pudo conectar, ya está llena la partida.";
		Logger::Instance()->LOG(ss.str(), LogTypeWarn);
		printf("%s \n", ss.str().c_str());
		pthread_mutex_unlock(&m_createPlayerMutex);
		return false;
	}

	int playerSpeed = m_parserNivel->getAvion().velDespl;
	int shootingCooldown = m_parserNivel->getAvion().cdDisp;
	int bulletsSpeed = m_parserNivel->getAvion().velDisp;

	Player* newPlayer = new Player();

	newPlayer->setPlayerTeam(m_teamsList[playerTeam]);
	newPlayer->setObjectID(clientID);
	newPlayer->setSpeed(Vector2D(playerSpeed, playerSpeed));

	newPlayer->setWeaponStats(bulletsSpeed, shootingCooldown, newPlayer->getObjectId(), newPlayer->getPlayerTeam().gameTeamID);

	m_playerNames[clientID] = playerName;

	ss << "player" << (clientID + 1);
	string playerStringID = ss.str();
	int playerTextureID = m_textureHelper->stringToInt(playerStringID);

	//14 HARDCODEADO
	newPlayer->load((m_gameWidth/2) - 19, m_gameHeight*0.75f, 38, 64, playerTextureID, 14);
	newPlayer->setConnected(true);

	m_listOfPlayer[newPlayer->getObjectId()]= newPlayer;

	printf("Player: %s inicializado con objectID: %d y textureID: %d\n",m_playerNames[clientID].c_str(), newPlayer->getObjectId(), clientID);
    printf("Player: %s pertenece al equipo: %s con TeamID: %i.",m_playerNames[clientID].c_str(), m_teamsList[playerTeam].gameTeamName.c_str(), m_teamsList[playerTeam].gameTeamID);

	CollitionHandler::Instance()->addPlayer(newPlayer);

	sendPlayerDataUpdate(newPlayer->getObjectId());

	pthread_mutex_unlock(&m_createPlayerMutex);
	return true;
}

void Game::addPowerUp(PowerUp* powerUp)
{
	m_powerUps.push_back(powerUp);
}

void Game::addEnemy(Enemy* enemy)
{
	m_enemies.push_back(enemy);
}

void Game::addPopUp(PopUp* popUp)
{
	m_popUps.push_back(popUp);
}

int Game::getFromNameID(const std::string& playerName)
{
	for (std::map<int, std::string>::iterator it = m_playerNames.begin(); it != m_playerNames.end(); ++it )
	{
		if (it->second.compare(playerName.c_str()) == 0)
			return it->first;
	}
	return -1;
}

bool Game::validatePlayerName(const std::string& playerName)
{
	for (std::map<int, std::string>::iterator it = m_playerNames.begin(); it != m_playerNames.end(); ++it )
	{
	    if (it->second.compare(playerName.c_str()) == 0)
	        return false;
	}
	//Nombre disponible
	return true;
}

Vector2D Game::getRandomPLayerCenter()
{
	int randomID = Random::getRange(0, m_listOfPlayer.size());
	Vector2D playerCenter;
	if ((m_listOfPlayer[randomID]) && (m_listOfPlayer[randomID]->isConnected()) && (!(m_listOfPlayer[randomID]->isDead())))
	{
		playerCenter = Vector2D(m_listOfPlayer[randomID]->getPosition().getX() + (m_listOfPlayer[randomID]->getWidth()/2),
								m_listOfPlayer[randomID]->getPosition().getY() + (m_listOfPlayer[randomID]->getHeight()/2));
		return playerCenter;
	}
	else
	{
		for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
		{

			if ((it->second) && (it->second->isConnected()) && (!(it->second->isDead())))
			{
				playerCenter = Vector2D(it->second->getPosition().getX() + (it->second->getWidth()/2),
										it->second->getPosition().getY() + (it->second->getHeight()/2));
				return playerCenter;
			}
		}
	}
	//No encontro ningun jugador
	playerCenter =  Vector2D( Game::Instance()->getGameWidth() / 2 , Game::Instance()->getGameHeight());
	return playerCenter;
}

void Game::disconnectPlayer(int playerID)
{
	PlayerDisconnection playerDiscMsg;
	std::size_t length = m_playerNames[playerID].copy(playerDiscMsg.name, MAX_NAME_LENGTH, 0);
	playerDiscMsg.name[length]='\0';
	playerDiscMsg.objectID = m_listOfPlayer[playerID]->getObjectId();
	playerDiscMsg.layer = m_listOfPlayer[playerID]->getLayer();
	m_server->informPlayerDisconnection(playerDiscMsg, playerID);

	m_listOfPlayer[playerID]->setConnected(false);
	//m_playerNames.erase(playerID);
	//listOfPlayer.erase(id);
	//mostrar en gris
}

void Game::render()
{
    //SDL_RenderClear(m_pRenderer);


    //Dibujar lo que haya que dibujar
 /*   m_background->draw(); //Provisorio
    m_island->draw(); //Provisorio
    m_player->draw();//Provisorio
*/
    //SDL_RenderPresent(m_pRenderer);
}


void Game::updateBackground(int scrollSpeed)
{
 	BackgroundInfo bgInfo;
 	if ((Game::Instance()->isLevelStarted() && (!Game::Instance()->isFinishingLevel())) || (m_scrollingToNextStage))
	{
		bgInfo.backgroundOffset = scrollSpeed;
	}
	else
	{
		bgInfo.backgroundOffset = 0;
	}
 	sendBackgroundInfo(bgInfo);
}

void Game::checkPracticeMode()
{
	if (m_practiceHoldTimer > 0)
	{
		m_practiceHoldTimer -= GameTimeHelper::Instance()->deltaTime();
	}

	if (m_practiceMode)
	{
		if (!CollitionHandler::Instance()->isPracticeMode())
			CollitionHandler::Instance()->setPracticeMode(true);
	}
	else
	{
		if (CollitionHandler::Instance()->isPracticeMode())
			CollitionHandler::Instance()->setPracticeMode(false);
	}
}

void Game::setPracticeMode(bool practiceMode)
{
	if (m_practiceHoldTimer > 0)
		return;
	m_practiceMode = practiceMode;
	m_practiceHoldTimer = 500;

}


bool Game::isPracticeMode()
{
	return m_practiceMode;
}

bool Game::isTeamMode()
{
 	return (m_parserNivel->getEscenario().gameMode == GameModeCompetition);
}

std::vector<GameTeam> Game::gameTeams() {
	return m_teamsList;
    //return m_parserNivel->getEscenario().teamsList;
}

bool Game::areAllPlayersDead()
{
	bool oneAlive = false;
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		//printf("objectID = %d \n", it->second.getObjectId());
		if ((it->second) && (it->second->isDead() == false))
		{
			oneAlive = true;
			break;
		}
	}
	return (!oneAlive);
}

void Game::initializeTexturesInfo()
{
	std::vector<Sprite> sprites = m_parserNivel->getListaSprites();
	for (std::vector<Sprite>::iterator it = sprites.begin() ; it !=  sprites.end(); ++it)
	{
		TextureInfo textureInfo;
		//id string to int
		textureInfo.textureID = m_textureHelper->stringToInt((*it).id);
		//path string to buffer
		std::size_t length = (*it).path.copy(textureInfo.path, PATH_MAX_LENGTH, 0);
		textureInfo.path[length]='\0';
		//otras variables de imagen
		textureInfo.numFrames = (*it).cantidad;
		textureInfo.height = (*it).alto;
		textureInfo.width = (*it).ancho;
		textureInfo.lastTexture = false;

		TextureManager::Instance()->addTextureInfo(textureInfo);
	}

}

void Game::setPlayersDirty()
{
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->setDirty(true);
		}
	}
}

void Game::changePlayerWeapon(Weapon* weapon, Player* player)
{
	player->setWeapon(weapon);

	if (m_parserNivel)
	{
		int playerSpeed = m_parserNivel->getAvion().velDespl;
		int shootingCooldown = m_parserNivel->getAvion().cdDisp;
		int bulletsSpeed = m_parserNivel->getAvion().velDisp;
		player->setWeaponStats(bulletsSpeed, shootingCooldown, player->getObjectId(), player->getPlayerTeam().gameTeamID);
	}

}

void Game::handleEvents()
{
}
void Game::inicializarServer()
{

	std::string fileName = "Utils/Default/servidor.xml";

	ParserServidor* servidorParser = new ParserServidor();
	servidorParser->parsearDocumento(fileName);

	LoggerInfo loggerInfo = servidorParser->getLoggerInfo();
	Logger::Instance()->setLoglevel(loggerInfo.debugAvailable, loggerInfo.warningAvailable, loggerInfo.errorAvailable);

	int porto = servidorParser->getServidorInfo().puerto ;


	int maxClientes = m_parserNivel->getEscenario().cantidadJugadores;

	m_server = new server(porto, maxClientes);

    m_drawMessagePacker = new DrawMessagesPacker(m_server);

	//printf("Se pone a escuchar\n");
	m_server->escuchar();

	int auxi = 0;
	while(m_server->getNumClientes() < m_server->getMaxClientes())
	{
		auxi++;
		m_server->aceptar();
		printf("cant clientes = %d / %d \n", m_server->getNumClientes(), m_server->getMaxClientes());
	}

	//Informa a los clientes que el juego comenzará
	m_server->informGameBeginning();

	keepListening();
}


void Game::sendToAllClients(DrawMessage mensaje)
{
	m_server->sendDrawMsgToAll(mensaje);
}

void Game::sendScoreToClients(ScoreMessage scoreMsg)
{
	m_server->sendScoreMsgToAll(scoreMsg);
}

void Game::sendBackgroundInfo(BackgroundInfo backgroundInfo)
{
	m_server->sendBackgroundInfoToAll(backgroundInfo);
}

void Game::sendStageBeginning(StageBeginning stageBeginningInfo)
{
	m_server->sendStageBeginningInfoToAll(stageBeginningInfo);
}

void Game::sendFinishGameInfo(FinishGameInfo finishGameInfo)
{
	m_server->sendFinishGameInfoToAll(finishGameInfo);
}

void Game::sendStageStatistics(StageStatistics stageStatistics, int clientID)
{
	m_server->sendStageStatistics(stageStatistics, clientID);
}

void Game::sendPlayerDataUpdate(int playerID)
{
	int playerLives = 0;
	PlayerDataUpdateInfo playerDataInfo;

	playerDataInfo.playerID = playerID;
	if (m_listOfPlayer[playerID])
	{
		playerLives = m_listOfPlayer[playerID]->getLives();
	}
	playerDataInfo.playerLives = playerLives;

	m_server->sendPlayerDataToClient(playerDataInfo);
}

void Game::addToPackage(DrawMessage drawMsg)
{
	m_drawMessagePacker->addDrawMessage(drawMsg);
}
void Game::sendPackages()
{
	m_drawMessagePacker->sendPackedMessages();
}

void Game::sendPlayerNames()
{
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			PlayerNameUpdateInfo playerNameInfo;
			playerNameInfo.playerID = it->first;

			std::size_t length = m_playerNames[it->first].copy(playerNameInfo.playerName, MAX_NAME_LENGTH, 0);
			playerNameInfo.playerName[length]='\0';

			m_server->sendPlayerNames(playerNameInfo);
		}
	}
}

void* Game::koreaMethod(void)
{
	while (m_server->isRunning())
	{
		m_server -> aceptar();
	}
	 pthread_exit(NULL);
}
void *Game::thread_method(void *context)
{
	return ((Game *)context)->koreaMethod();
}
void Game::keepListening()
{
	pthread_create(&listenThread, NULL, &Game::thread_method, (void*)this);

}

void Game::addPointsToScore(int points, int playerID, int teamID)
{
	m_listOfPlayer[playerID]->addPoints(points);
	addPointsToTeam(points, teamID);
	printf("jugador %s sumó %d puntos!\n", m_playerNames[playerID].c_str(), points);
	printf ("Puntos totales = %d \n", m_listOfPlayer[playerID]->getScore());
}

void Game::addPointsToTeam(int points, int teamID)
{
	//printf("sumando %d puntos al equipo %d\n", points, teamID);
	if ((teamID != 0) && (teamID != 1))
			return;
	m_teamsList[teamID].gameTeamScore += points;
    if (m_teamsList[teamID].gameTeamScore < 0)
    {
    	m_teamsList[teamID].gameTeamScore = 0;
    }
   // std::vector<GameTeam> teams = this->m_parserNivel->getEscenario().teamsList;
    /*std::vector<GameTeam>::iterator iterator = m_teamsList.begin();
    
    for (*iterator; iterator != m_teamsList.end(); ++iterator) {
       // GameTeam playerTeam = *iterator;
        
        if ((*iterator).gameTeamID == teamID)
        {
        	(*iterator).gameTeamScore = (*iterator).gameTeamScore + points;
            if ((*iterator).gameTeamScore < 0)
            {
            	(*iterator).gameTeamScore = 0;
            }
        }
    }*/
}

Player* Game::getPlayer(int playerID)
{
	if (m_listOfPlayer[playerID])
	{
		return m_listOfPlayer[playerID];
	}

	return NULL;
}

int Game::getPlayerTeam(int playerID)
{
	if (m_listOfPlayer.find(playerID) == m_listOfPlayer.end())
	{
		return -1;
	}

	return (m_listOfPlayer[playerID]->getPlayerTeam().gameTeamID);
}

void Game::actualizarEstado(int id, InputMessage inputMsg){
	pthread_mutex_lock(&m_updatePlayerMutex);

	m_listOfPlayer[inputMsg.objectID]->handleInput(inputMsg);

	pthread_mutex_unlock(&m_updatePlayerMutex);

}

void Game::clean()
{
    cout << "cleaning game\n";
    BulletsHandler::Instance()->clearBullets();

	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			 it->second->clean();
			 delete  it->second;
		}
	}
	 for (std::vector<Enemy*>::iterator it = m_enemies.begin() ; it != m_enemies.end(); ++it)
	 {
		 (*it)->clean();
		 delete (*it);
	 }
	 for (std::vector<PowerUp*>::iterator it = m_powerUps.begin() ; it != m_powerUps.end(); ++it)
	 {
		 (*it)->clean();
		 delete (*it);
	 }

	 for (std::vector<PopUp*>::iterator it = m_popUps.begin() ; it != m_popUps.end(); ++it)
	 {
		 (*it)->clean();
		 delete (*it);
	 }

	for (std::map<int,GameObject*>::iterator it=m_listOfGameObjects.begin(); it != m_listOfGameObjects.end(); ++it)
	{
		if (it->second)
		{
			 it->second->clean();
			 delete  it->second;
		}
	}
    m_listOfPlayer.clear();
    m_listOfGameObjects.clear();
    m_playerNames.clear();

    m_drawMessagePacker->clean();
    delete m_drawMessagePacker;

    m_parserNivel->clean();
    delete m_parserNivel;
    m_parserStages->clean();
    delete m_parserStages;

    m_level->clean();
    delete m_level;
    delete m_textureHelper;

    CollitionHandler::Instance()->clean();

    InputHandler::Instance()->clean();
    TextureManager::Instance()->clearTextureMap();

    m_powerUpsSpawner->clean();
    m_enemiesSpawner->clean();
    delete m_powerUpsSpawner;
    delete m_enemiesSpawner;

    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}

void Game::refreshPlayersDirty()
{
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->setDirty(true);
		}
	}
}

void Game::resetGame()
{
	pthread_mutex_lock(&m_resetMutex);
	 BulletsHandler::Instance()->clearBullets();
	 InputHandler::Instance()->clean();
	 CollitionHandler::Instance()->reset();
	 //delete m_background;
	 //delete m_island;
	 //m_listOfGameObjects.clear();
	 if (m_level)
	 {
		 m_level->clean();
		 delete m_level;
	 }
	 m_parserNivel->clean();

	 //CARGAR XML
	 m_parserNivel = new ParserNivel();
	 m_parserNivel->parsearDocumento(XML_PATH);
	 m_gameWidth = m_parserNivel->getVentana().ancho;
	 m_gameHeight = m_parserNivel->getVentana().alto;

	 m_level = new Level();
	 m_level->loadFromXML(XML_PATH);

	int newPlayerSpeed = m_parserNivel->getAvion().velDespl;
	int newShootingCooldown = m_parserNivel->getAvion().cdDisp;
	int newBulletsSpeed = m_parserNivel->getAvion().velDisp;
	for (std::map<int, Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->reset();
			it->second->setSpeed(Vector2D(newPlayerSpeed, newPlayerSpeed));
			it->second->setShootingCooldown(newShootingCooldown);
			it->second->setShootingSpeed(newBulletsSpeed);
			it->second->refreshDirty();
			it->second->StopFlipAnimation();
			it->second->setPosition(Vector2D(Game::Instance()->getGameWidth()/2 - 32,  m_gameHeight - m_gameHeight/5 ));
		}
	}

	 //tudo ben
	 m_running = true;
	 pthread_mutex_unlock(&m_resetMutex);
}

//Mata a todos los enemigos y suma puntos al responsable
void  Game::killAllEnemies(Player* killer)
{
	 for (std::vector<Enemy*>::iterator it = m_enemies.begin() ; it != m_enemies.end();++it)
	 {
		 if((*it) && ((*it)->isDying() == false) && ((*it)->isDead() == false))
		 {
			 (*it)->damage(50000, false, killer);
		 }
	 }
}

void Game::killAllEnemiesNoRewards()
{
	 for (std::vector<Enemy*>::iterator it = m_enemies.begin() ; it != m_enemies.end();++it)
	 {
		 if((*it) && ((*it)->isDying() == false) && ((*it)->isDead() == false))
		 {
			 (*it)->kill();
		 }
	 }
}

void Game::cleanDeadObjects()
{
	 for (std::vector<Enemy*>::iterator it = m_enemies.begin() ; it != m_enemies.end();)
	 {
	   if((*it) && ((*it)->canRecycle()) && ((*it)->isDead()))
	   {
			(*it)->clean();
			delete (*it);
			it = m_enemies.erase(it);
	   }
	   else
	   {
		   ++it;
	   }
	 }
	 for (std::vector<PowerUp*>::iterator it = m_powerUps.begin() ; it != m_powerUps.end();)
	 {
		 if((*it) && ((*it)->canRecycle()) && ((*it)->isDead()))
		 {
			 (*it)->clean();
			 delete (*it);
			 it = m_powerUps.erase(it);
		 }
		 else
		 {
			 ++it;
		 }
	 }

	 for (std::vector<PopUp*>::iterator it = m_popUps.begin() ; it != m_popUps.end();)
	 {
		 if((*it) && ((*it)->canRecycle()) && ((*it)->isDead()))
		 {
			 (*it)->clean();
			 delete (*it);
			 it = m_popUps.erase(it);
		 }
		 else
		 {
			 ++it;
		 }
	 }
}

/*Carga los objetos que deba cargar, dependiendo de la posicion del mapa*/
void Game::updateSpawners()
{
	m_enemiesSpawner->update(m_level->getVirtualPosition());
	m_powerUpsSpawner->update(m_level->getVirtualPosition());
}

void Game::update()
{

	checkStartingStage(); // espera la animacion de vuelta
	checkEndingStage(); // si esta terminando el nivel, espera un tiempo y carga el siguiente
	checkStageTransitioning();

	checkPracticeMode();

	/*if (!m_gameStarted)
		return;
	 */
	updateSpawners();

	BulletsHandler::Instance()->updateBullets();
	//enemy->update();

	m_level->update();

	updateBackground(m_level->getScrollSpeed());

	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		//printf("objectID = %d \n", it->second.getObjectId());
		if (it->second)
		{
			it->second->update();
		}
	}
	 for (std::vector<Enemy*>::iterator it = m_enemies.begin() ; it != m_enemies.end(); ++it)
	 {
		 (*it)->update();
	 }
	 for (std::vector<PowerUp*>::iterator it = m_powerUps.begin() ; it != m_powerUps.end(); ++it)
	 {
			(*it)->update();
	 }

	for (std::map<int,GameObject*>::iterator it=m_listOfGameObjects.begin(); it != m_listOfGameObjects.end(); ++it)
	{
		//printf("objectID = %d \n", it->second.getObjectId());
		if (it->second)
		{
			it->second->update();
		}
	}

	 for (std::vector<PopUp*>::iterator it = m_popUps.begin() ; it != m_popUps.end(); ++it)
	 {
			(*it)->update();
	 }

	if ((!m_gameOver) && (areAllPlayersDead()))
	{
		//INFORMAR DERROTA O JUGADOR GANADOR
		printf("Game Over \n");
		informEndGame(false);
		m_scrollBackground = false;
		m_gameOver = true;
		m_endingStage = false;
		m_level->setScrollSpeed(0);

	}

	CollitionHandler::Instance()->handleCollitions();

	cleanDeadObjects();

}

void Game::informEndGame(bool levelFinished)
{
	FinishGameInfo finishGameInfo;
	if (m_parserNivel->getEscenario().gameMode == GameModeCooperative)
	{
		int maxScore = -1;
		int winnerID = 0;
		finishGameInfo.isVictory = levelFinished;
		for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
		{
			if (it->second)
			{
				int score = it->second->getScore();
				if (score >= maxScore)
				{
					maxScore = score;
					winnerID = it->second->getObjectId();
				}
			}
		}
		printf("Max Score %d \n", maxScore);
		finishGameInfo.points = maxScore;
		finishGameInfo.winnerID = winnerID;

	}
	if (m_parserNivel->getEscenario().gameMode == GameModeCompetition)
	{
		int maxScore = -1;
		int winnerID = 0;
		finishGameInfo.isVictory = true;
        
        //std::vector<GameTeam> teamList = this->m_parserNivel->getEscenario().teamsList;
       /*for (std::vector<GameTeam>::iterator iterator = m_teamsList.begin(); iterator != m_teamsList.end(); ++iterator)
        {
			GameTeam team = *iterator;
			if (team.gameTeamScore >= maxScore)
			{
				maxScore = team.gameTeamScore;
				winnerID = team.gameTeamID;
			}
		}*/
        for (unsigned int i = 0; i < m_teamsList.size(); ++i)
        {
        	//printf("score Team %d = %d\n",  m_teamsList[i].gameTeamID,  m_teamsList[i].gameTeamScore);
			if (m_teamsList[i].gameTeamScore >= maxScore)
			{
				maxScore = m_teamsList[i].gameTeamScore;
				winnerID = m_teamsList[i].gameTeamID;
			}
        }
        printf("Max Score %d \n", maxScore);
		finishGameInfo.points = maxScore;
		if (maxScore == 0)
		{
 			finishGameInfo.winnerID = -1;
		}
		else
		{
 			finishGameInfo.winnerID = winnerID;
		}
	}

	sendFinishGameInfo(finishGameInfo);
}

void Game::checkStageTransitioning()
{
	if (m_waitingToScroll)
	{
		if (!m_level->isScrollingToNextStage())
		{
			for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
			{
				if (it->second)
				{
					it->second->setPosition(Vector2D(Game::Instance()->getGameWidth()/2 - 32, m_gameHeight*0.75f));
				}
			}
			++m_currentStage;
			loadCurrentStage();
			//m_level->loadLevel(m_currentStage);

			m_waitingToScroll = false;

			startStage();
		}
	}
}

void Game::checkEndingStage()
{
	if (m_endingStage)
	{
		m_waitEndStageTimer -= GameTimeHelper::Instance()->deltaTime();
		if (m_waitEndStageTimer <= 0)
		{
			//loadNextStage();
			doStageTransition();
			m_endingStage = false;
		}
	}
}

void Game::checkStartingStage()
{
	if (m_startingStage)
	{
		bool animationDone = false;
		for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
		{
			//printf("objectID = %d \n", it->second.getObjectId());
			if ((it->second) && (it->second->isDoingFlip() == false))
			{
				animationDone = true;
				break;
			}
		}
		if (animationDone)
		{
			if ((m_startingWaitTime <= 0) && (!m_waitingToScroll))
			{
				m_startingStage = false;
				m_scrollBackground = true;

			}
		}
		else
		{
			m_startingWaitTime -= GameTimeHelper::Instance()->deltaTime();
		}

	}
}

void Game::loadCurrentStage()
{
	if (m_parserStages)
	{
		m_parserStages->clean();
		delete m_parserStages;
        m_parserStages = NULL;
	}

	m_parserStages = new ParserStage();
	std::stringstream ss;
	ss <<"Stage" << m_currentStage << ".xml";

	m_parserStages->parsearDocumento(ss.str());

	m_enemiesSpawner->feed(m_parserStages->getListaDeEnemigos(), m_parserStages->getVentana().alto);
	m_powerUpsSpawner->feed(m_parserStages->getListaDePowerups(), m_parserStages->getVentana().alto);

	printf("Se cargó el Stage %d \n", m_currentStage);

	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->resetStageStatistics();
			//it->second->startFlipAnimation();
		}
	}

}

void Game::doStageTransition()
{
	m_scrollingToNextStage = true;
	m_waitingToScroll = true;
	m_scrollBackground = true;
	m_level->scrollToNextStage();
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->moveAutomatic(Vector2D(Game::Instance()->getGameWidth()/2 - 32, m_gameHeight*0.75f), m_scrollSpeed);
		}
	}

}

void Game::loadNextStage()
{
	if (m_currentStage >= m_stagesAmount)
	{
		return;
	}
	printf("Loading stage %d \n", m_currentStage + 1);

	++m_currentStage;
	loadCurrentStage();
	m_level->resetPositions();

	printf("Stage %d loaded.\n", m_currentStage);
}

void Game::finishStage()
{
	printf("Ending stage %d \n", m_currentStage);
	killAllEnemiesNoRewards();

	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->moveAutomatic(Vector2D((Game::Instance()->getGameWidth()/2) - (it->second->getWidth()/2), m_gameHeight/5), 4);

			StageStatistics stageStatistics;
			stageStatistics.accuracy = static_cast<short>(it->second->getStageStatistics().getAccuracy() * 100);
			//stageStatistics.accuracy = stageStatistics.accuracy % 100;
			stageStatistics.enemiesKilled = static_cast<short>(it->second->getStageStatistics().getEnemiesKilled());
			stageStatistics.points = static_cast<short>(it->second->getStageStatistics().getPoints());
			stageStatistics.something = 0;
			//printf("acc %d - points %d, enemyKilled %d", stageStatistics.accuracy, 	stageStatistics.points, stageStatistics.enemiesKilled);

			sendStageStatistics(stageStatistics, it->second->getObjectId());
		}
	}

	m_scrollBackground = false;
	m_endingStage = true;
	m_waitEndStageTimer = END_STAGE_TIMER;

	printf("Stage %d ended. \n", m_currentStage);

	if (m_currentStage >= m_stagesAmount)
	{
		printf("End of Level \n");
		informEndGame(true);
		m_scrollBackground = false;
		m_gameOver = true;
		m_endingStage = false;
		m_level->setScrollSpeed(0);
	}
}

void Game::startStage()
{
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->startFlipAnimation();
		}
	}
	m_stageStarted = true;
	m_scrollBackground = false;

	m_startingStage = true;
	m_startingWaitTime = 500;

	StageBeginning stageBeginningInfo;
	stageBeginningInfo.stageNumber = m_currentStage;
	sendStageBeginning(stageBeginningInfo);
}


void Game::restartLevel()
{
	for (std::map<int,Player*>::iterator it=m_listOfPlayer.begin(); it != m_listOfPlayer.end(); ++it)
	{
		if (it->second)
		{
			it->second->reset();
			it->second->refreshDirty();
			it->second->setPosition(Vector2D(Game::Instance()->getGameWidth()/2 - 32,  m_gameHeight - m_gameHeight/5 ));

		}
	}

	 /*for (std::vector<Enemy*>::iterator it = m_enemies.begin() ; it != m_enemies.end(); ++it)
	 {
		 //(*it)->clean();
		 (*it)->setDead(true);
	 }*/
	killAllEnemiesNoRewards();
	 for (std::vector<PowerUp*>::iterator it = m_powerUps.begin() ; it != m_powerUps.end(); ++it)
	 {
		 //(*it)->clean();
		 (*it)->setDead(true);
	 }

	 for (std::vector<PopUp*>::iterator it = m_popUps.begin() ; it != m_popUps.end(); ++it)
	 {
		 //(*it)->clean();
		 (*it)->setDead(true);
	 }
	 m_endingStage = false;
	 m_gameOver = false;
	m_scrollBackground = true;

	m_level->setScrollSpeed(m_scrollSpeed);

	 m_level->reset();
	 m_powerUpsSpawner->reset();
	 m_enemiesSpawner->reset();
     for (unsigned int i = 0; i < m_teamsList.size(); ++i)
     {
    	 m_teamsList[i].gameTeamScore = 0;
     }

	 m_currentStage = 1;
	 loadCurrentStage();
	 startStage();
}
