/*
 * CodificadorMensajes.cpp
 *
 *  Created on: Apr 2, 2016
 *      Author: gonzalo
 */

#include "AlanTuring.h"

AlanTuring::AlanTuring()
{

}
AlanTuring::~AlanTuring()
{

}

/********************************************Metodos del Juego*********************************************************************/
int AlanTuring::encodeDrawMessage(DrawMessage drawMsg, char* bufferSalida)
{
	//inicializa el buffer de salida
	bzero(bufferSalida,MESSAGE_BUFFER_SIZE);

	NetworkMessage codigoEnigma;
	bzero(codigoEnigma.msg_Data, MESSAGE_DATA_SIZE);

	codigoEnigma.msg_Code[0] = 'd';
	codigoEnigma.msg_Code[1] = 'm';
	codigoEnigma.msg_Code[2] = 's';

	//copia el draw message en el buffer de network message data
	memcpy(codigoEnigma.msg_Data, &drawMsg, sizeof(DrawMessage));

	codigoEnigma.msg_Length = sizeof(DrawMessage) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	//copia el mensaje de red al buffer ingresado
	memcpy(bufferSalida, &codigoEnigma, sizeof(NetworkMessage));

	return codigoEnigma.msg_Length;
}

int AlanTuring::encodeDrawMessagePackage(DrawMessagePack packMsg, char* bufferEntrada)
{
	bzero(bufferEntrada,MESSAGE_BUFFER_SIZE);

	NetworkMessage codigoEnigma;
	bzero(codigoEnigma.msg_Data, MESSAGE_DATA_SIZE);

	codigoEnigma.msg_Code[0] = 'd';
	codigoEnigma.msg_Code[1] = 'm';
	codigoEnigma.msg_Code[2] = 'p';

	int drawMessagesAmount = (packMsg.totalSize - sizeof(int)) / sizeof(DrawMessage);
	int packSize = (drawMessagesAmount * sizeof(DrawMessage)) + sizeof(int);
	//copia el draw message en el buffer de network message data
	memcpy(codigoEnigma.msg_Data, &packMsg,  sizeof(DrawMessagePack));

	codigoEnigma.msg_Length =  packSize + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	//copia el mensaje de red al buffer ingresado
	memcpy(bufferEntrada, &codigoEnigma, sizeof(NetworkMessage));

	return codigoEnigma.msg_Length;
}

int AlanTuring::encodeInputMessage(InputMessage inputMsg, char* bufferSalida)
{
	//inicializa el buffer de salida
	bzero(bufferSalida, MESSAGE_BUFFER_SIZE);

	NetworkMessage codigoEnigma;
	bzero(codigoEnigma.msg_Data, MESSAGE_DATA_SIZE);

	codigoEnigma.msg_Code[0] = 'i';
	codigoEnigma.msg_Code[1] = 'm';
	codigoEnigma.msg_Code[2] = 's';


	//copia el draw message en el buffer de network message data
	memcpy(codigoEnigma.msg_Data, &inputMsg, sizeof(InputMessage));

	codigoEnigma.msg_Length = sizeof(InputMessage) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	//copia el mensaje de red al buffer ingresado
	memcpy(bufferSalida, &codigoEnigma, sizeof(NetworkMessage));

	return codigoEnigma.msg_Length;
}
int AlanTuring::encodeConnectedMessage(ConnectedMessage connectedMsg, char* bufferSalida)
{
	//inicializa el buffer de salida
	bzero(bufferSalida, MESSAGE_BUFFER_SIZE);

	NetworkMessage codigoEnigma;
	bzero(codigoEnigma.msg_Data, MESSAGE_DATA_SIZE);

	codigoEnigma.msg_Code[0] = 'c';
	codigoEnigma.msg_Code[1] = 'n';
	codigoEnigma.msg_Code[2] = 't';


	//copia el draw message en el buffer de network message data
	memcpy(codigoEnigma.msg_Data, &connectedMsg, sizeof(ConnectedMessage));

	codigoEnigma.msg_Length = sizeof(ConnectedMessage) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	//copia el mensaje de red al buffer ingresado
	memcpy(bufferSalida, &codigoEnigma, sizeof(NetworkMessage));

	return codigoEnigma.msg_Length;
}

int AlanTuring::encodeConnectionInfoMessage(ConnectionInfo connectionInfoMesg, char* bufferEntrada)
{
	bzero(bufferEntrada, MESSAGE_BUFFER_SIZE);

	NetworkMessage codigoEnigma;
	bzero(codigoEnigma.msg_Data, MESSAGE_DATA_SIZE);

	codigoEnigma.msg_Code[0] = 'c';
	codigoEnigma.msg_Code[1] = 'n';
	codigoEnigma.msg_Code[2] = 'i';


	//copia el draw message en el buffer de network message data
	memcpy(codigoEnigma.msg_Data, &connectionInfoMesg, sizeof(ConnectionInfo));

	codigoEnigma.msg_Length = sizeof(ConnectionInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	//copia el mensaje de red al buffer ingresado
	memcpy(bufferEntrada, &codigoEnigma, sizeof(NetworkMessage));

	return codigoEnigma.msg_Length;
}

int AlanTuring::encodePlayerDisconnectionMessage(PlayerDisconnection playerDiscMsg, char* bufferEntrada)
{
	bzero(bufferEntrada, MESSAGE_BUFFER_SIZE);

	NetworkMessage codigoEnigma;
	bzero(codigoEnigma.msg_Data, MESSAGE_DATA_SIZE);

	codigoEnigma.msg_Code[0] = 'p';
	codigoEnigma.msg_Code[1] = 'd';
	codigoEnigma.msg_Code[2] = 'c';


	//copia el draw message en el buffer de network message data
	memcpy(codigoEnigma.msg_Data, &playerDiscMsg, sizeof(PlayerDisconnection));

	codigoEnigma.msg_Length = sizeof(PlayerDisconnection) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	//copia el mensaje de red al buffer ingresado
	memcpy(bufferEntrada, &codigoEnigma, sizeof(NetworkMessage));

	return codigoEnigma.msg_Length;
}

DrawMessage AlanTuring::decodeDrawMessage (NetworkMessage netMsg)
{
	DrawMessage drawMsg;
	memcpy(&drawMsg, netMsg.msg_Data, sizeof(DrawMessage));
	return drawMsg;
}
InputMessage AlanTuring::decodeInputMessage (NetworkMessage netMsg)
{
	InputMessage inputMsg;
	memcpy(&inputMsg, netMsg.msg_Data, sizeof(InputMessage));
	return inputMsg;
}
ConnectedMessage AlanTuring::decodeConnectedMessage (NetworkMessage netMsg)
{
	ConnectedMessage connectedMsg;
	memcpy(&connectedMsg, netMsg.msg_Data, sizeof(ConnectedMessage));
	return connectedMsg;
}

ConnectionInfo AlanTuring::decodeConnectionInfoMessage(NetworkMessage netMsg)
{
	ConnectionInfo connectionInfoMsg;
	memcpy(&connectionInfoMsg, netMsg.msg_Data, sizeof(ConnectionInfo));
	return connectionInfoMsg;
}

TextureInfo  AlanTuring::decodeTextureInfo(NetworkMessage netMsg)
{
	TextureInfo textureInfoMsg;
	memcpy(&textureInfoMsg, netMsg.msg_Data, sizeof(TextureInfo));
	return textureInfoMsg;
}
PlayerReconnectionInfo AlanTuring::decodePlayerReconnectionInfo(NetworkMessage netMsg)
{
	PlayerReconnectionInfo playerReconnectionInfo;
	memcpy(&playerReconnectionInfo, netMsg.msg_Data, sizeof(PlayerReconnectionInfo));
	return playerReconnectionInfo;
}

ScoreMessage AlanTuring::decodeScoreMessage(NetworkMessage netMsg)
{
	ScoreMessage scoreMessage;
	memcpy(&scoreMessage, netMsg.msg_Data, sizeof(ScoreMessage));
	return scoreMessage;
}

BackgroundInfo AlanTuring::decodeBackgroundInfo(NetworkMessage netMsg)
{
	BackgroundInfo backgroundInfo;
	memcpy(&backgroundInfo, netMsg.msg_Data, sizeof(BackgroundInfo));
	return backgroundInfo;
}

StageStatistics AlanTuring::decodeStageStatistics(NetworkMessage netMsg)
{
	StageStatistics stageStatistics;
	memcpy(&stageStatistics, netMsg.msg_Data, sizeof(StageStatistics));
	return stageStatistics;
}

FinishGameInfo AlanTuring::decodeFinishGameInfo(NetworkMessage netMsg)
{
	FinishGameInfo finishGameInfo;
	memcpy(&finishGameInfo, netMsg.msg_Data, sizeof(FinishGameInfo));
	return finishGameInfo;
}

StageBeginning AlanTuring::decodeStageBeginning(NetworkMessage netMsg)
{
	StageBeginning stageBeginningInfo;
	memcpy(&stageBeginningInfo, netMsg.msg_Data, sizeof(StageBeginning));
	return stageBeginningInfo;
}

PlayerNameUpdateInfo AlanTuring::decodePlayerNameUpdateInfo(NetworkMessage netMsg)
{
	PlayerNameUpdateInfo playerNameUpdateInfo;
	memcpy(&playerNameUpdateInfo, netMsg.msg_Data, sizeof(PlayerNameUpdateInfo));
	return playerNameUpdateInfo;
}
PlayerDataUpdateInfo AlanTuring::decodePlayerDataUpdateInfo(NetworkMessage netMsg)
{
	PlayerDataUpdateInfo playerDataUpdateInfo;
	memcpy(&playerDataUpdateInfo, netMsg.msg_Data, sizeof(PlayerDataUpdateInfo));
	return playerDataUpdateInfo;
}


PlayerDisconnection AlanTuring::decodePlayerDisconnectionMessage(NetworkMessage netMsg)
{
	PlayerDisconnection playerDiscMsg;
	memcpy(&playerDiscMsg, netMsg.msg_Data, sizeof(PlayerDisconnection));
	return playerDiscMsg;
}

DrawMessagePack AlanTuring::decodeDrawMessagePackage(NetworkMessage netMsg)
{
	int messageLength = 0;
	memcpy(&messageLength, &netMsg, sizeof(messageLength));

	DrawMessagePack drawMsgPack;
	memcpy(&drawMsgPack, netMsg.msg_Data, messageLength);
	return drawMsgPack;
}

ResetInfo AlanTuring::decodeResetInfo(NetworkMessage netMsg)
{
	ResetInfo resetInfoMsg;
	memcpy(&resetInfoMsg, netMsg.msg_Data, sizeof(ResetInfo));
	return resetInfoMsg;
}

NetworkMessage AlanTuring::drawMessageToNetwork(DrawMessage drawMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'd';
	networkMessage.msg_Code[1] = 'm';
	networkMessage.msg_Code[2] = 's';

	memcpy(networkMessage.msg_Data, &drawMessage, sizeof(DrawMessage));
	networkMessage.msg_Length = sizeof(DrawMessage) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}
NetworkMessage AlanTuring::playerDisconnectionToNetwork(PlayerDisconnection playerDiscMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'p';
	networkMessage.msg_Code[1] = 'd';
	networkMessage.msg_Code[2] = 'c';

	memcpy(networkMessage.msg_Data, &playerDiscMessage, sizeof(PlayerDisconnection));
	networkMessage.msg_Length = sizeof(PlayerDisconnection) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}


NetworkMessage AlanTuring::TextureInfoToNetwork(TextureInfo textureInfoMsg)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 't';
	networkMessage.msg_Code[1] = 'x';
	networkMessage.msg_Code[2] = 'i';

	memcpy(networkMessage.msg_Data, &textureInfoMsg, sizeof(TextureInfo));
	networkMessage.msg_Length = sizeof(TextureInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::PlayerReconnectionInfoToNetwork(PlayerReconnectionInfo playerReconnectionInfo)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'p';
	networkMessage.msg_Code[1] = 'r';
	networkMessage.msg_Code[2] = 'i';

	memcpy(networkMessage.msg_Data, &playerReconnectionInfo, sizeof(PlayerReconnectionInfo));
	networkMessage.msg_Length = sizeof(PlayerReconnectionInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::drawMsgPackToNetwork(DrawMessagePack drawMsgPack)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'd';
	networkMessage.msg_Code[1] = 'm';
	networkMessage.msg_Code[2] = 'p';

	int drawMessagesAmount = (drawMsgPack.totalSize - sizeof(int)) / sizeof(DrawMessage);
	int packSize = (drawMessagesAmount * sizeof(DrawMessage)) + sizeof(int);

	//printf("Tamaño total = %d \n", drawMsgPack.totalSize);
	//printf("%d x 24 + 4 = Tamaño paquete = %d",drawMessagesAmount, packSize);

	memcpy(networkMessage.msg_Data, &drawMsgPack, packSize);
	networkMessage.msg_Length =  packSize + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::ResetMsgToNetwork(ResetInfo resetMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'r';
	networkMessage.msg_Code[1] = 's';
	networkMessage.msg_Code[2] = 't';

	memcpy(networkMessage.msg_Data, &resetMessage, sizeof(ResetInfo));
	networkMessage.msg_Length = sizeof(ResetInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::ScoreMessageToNetwork(ScoreMessage scoreMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 's';
	networkMessage.msg_Code[1] = 'c';
	networkMessage.msg_Code[2] = 'm';

	memcpy(networkMessage.msg_Data, &scoreMessage, sizeof(ScoreMessage));
	networkMessage.msg_Length = sizeof(ScoreMessage) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::BackgroundInfoToNetwork(BackgroundInfo backgroundMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'b';
	networkMessage.msg_Code[1] = 'g';
	networkMessage.msg_Code[2] = 'i';

	memcpy(networkMessage.msg_Data, &backgroundMessage, sizeof(BackgroundInfo));
	networkMessage.msg_Length = sizeof(BackgroundInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}
NetworkMessage AlanTuring::StageStatisticsToNetwork(StageStatistics stageStatisticsMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 's';
	networkMessage.msg_Code[1] = 't';
	networkMessage.msg_Code[2] = 't';

	memcpy(networkMessage.msg_Data, &stageStatisticsMessage, sizeof(StageStatistics));
	networkMessage.msg_Length = sizeof(StageStatistics) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::FinishGameInfoToNetwork(FinishGameInfo finishGameInfo)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'f';
	networkMessage.msg_Code[1] = 'g';
	networkMessage.msg_Code[2] = 'i';

	memcpy(networkMessage.msg_Data, &finishGameInfo, sizeof(FinishGameInfo));
	networkMessage.msg_Length = sizeof(FinishGameInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::StageBeginningToNetwork(StageBeginning stageBeginningMessage)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 's';
	networkMessage.msg_Code[1] = 'b';
	networkMessage.msg_Code[2] = 'm';

	memcpy(networkMessage.msg_Data, &stageBeginningMessage, sizeof(StageBeginning));
	networkMessage.msg_Length = sizeof(StageBeginning) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

NetworkMessage AlanTuring::playerNameUpdateInfoToNetwork(PlayerNameUpdateInfo playerNameUpdateInfo)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'p';
	networkMessage.msg_Code[1] = 'n';
	networkMessage.msg_Code[2] = 'u';

	memcpy(networkMessage.msg_Data, &playerNameUpdateInfo, sizeof(PlayerNameUpdateInfo));
	networkMessage.msg_Length = sizeof(PlayerNameUpdateInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}
NetworkMessage AlanTuring::playerDataUpdateInfoToNetwork(PlayerDataUpdateInfo playerDataUpdateInfo)
{
	NetworkMessage networkMessage;
	bzero(networkMessage.msg_Data, MESSAGE_DATA_SIZE);
	networkMessage.msg_Code[0] = 'p';
	networkMessage.msg_Code[1] = 'd';
	networkMessage.msg_Code[2] = 'u';

	memcpy(networkMessage.msg_Data, &playerDataUpdateInfo, sizeof(PlayerDataUpdateInfo));
	networkMessage.msg_Length = sizeof(PlayerDataUpdateInfo) + MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

	return networkMessage;
}

/**********************************************************************************************************************************/

int AlanTuring::encodeXMLMessage(Mensaje mensaje, char* bufferSalida)
{
	//inicializa el buffer de salida
	bzero(bufferSalida,MESSAGE_BUFFER_SIZE);

	//codifica el mensaje en a un mensaje de Red
	NetworkMessage codigoEnigma;
	fillMsgData(&codigoEnigma, mensaje);
	int msgLength = codigoEnigma.msg_Length;
	//copia el mensaje de red al buffer de salida
	memcpy(bufferSalida, &codigoEnigma, sizeof(NetworkMessage));

	return msgLength;
}

int AlanTuring::encodeNetworkMessage(NetworkMessage netMsg, char* bufferSalida )
{
	bzero(bufferSalida,MESSAGE_BUFFER_SIZE);
	int length = netMsg.msg_Length;
	memcpy(bufferSalida, &netMsg, sizeof(NetworkMessage));
	return length;

}

NetworkMessage AlanTuring::decode(char* codigoEnigma)
{
	NetworkMessage netMsg;
	memcpy(&netMsg, codigoEnigma, sizeof(NetworkMessage));
	return netMsg;
}

/*
 * METODOS DE DECODIFICACION
 */
int AlanTuring::decodeLength(char* codigoEnigma)
{
	int messageLength = 0;
	memcpy(&messageLength, codigoEnigma, sizeof(messageLength));
	return messageLength;
}

DataMessage AlanTuring::decodeMessage (NetworkMessage netMsg)
{
	DataMessage dataMsg;
	memcpy(&dataMsg, netMsg.msg_Data, sizeof(DataMessage));

	std::string msgID(dataMsg.msg_ID);

	int pos = msgID.find("$");
	string id(msgID.substr(0, pos));

	msgID.erase(0, pos + 1);

	string valor(msgID);

	memcpy(dataMsg.msg_ID, id.c_str(), MESSAGE_ID_BYTES_LIMIT);
	memcpy(dataMsg.msg_value, valor.c_str(), MESSAGE_VALUE_SIZE);

	return dataMsg;
}


/*
 * 	METODOS DE CODIFICACION
 */
void AlanTuring::fillMsgData(NetworkMessage* netMsg, const Mensaje mensaje)
{
	bzero(netMsg->msg_Data,MESSAGE_DATA_SIZE);

	if ((strcmp(mensaje.tipo.c_str(), "int")) == 0)
	{
		netMsg->msg_Code[0] = 'i';
		netMsg->msg_Code[1] = 'n';
		netMsg->msg_Code[2] = 't';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "char")) == 0)
	{
		netMsg->msg_Code[0] = 'c';
		netMsg->msg_Code[1] = 'h';
		netMsg->msg_Code[2] = 'r';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "double")) == 0)
	{
		netMsg->msg_Code[0] = 'd';
		netMsg->msg_Code[1] = 'b';
		netMsg->msg_Code[2] = 'l';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "string")) == 0)
	{
		netMsg->msg_Code[0] = 's';
		netMsg->msg_Code[1] = 't';
		netMsg->msg_Code[2] = 'r';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "exit")) == 0)
	{
		netMsg->msg_Code[0] = 'e';
		netMsg->msg_Code[1] = 'x';
		netMsg->msg_Code[2] = 't';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "connected")) == 0)
	{
		netMsg->msg_Code[0] = 'c';
		netMsg->msg_Code[1] = 'n';
		netMsg->msg_Code[2] = 't';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "serverfull")) == 0)
	{
		netMsg->msg_Code[0] = 'f';
		netMsg->msg_Code[1] = 'u';
		netMsg->msg_Code[2] = 'l';
		encodeMessage(netMsg, mensaje);
	}
	if ((strcmp(mensaje.tipo.c_str(), "timeoutACK")) == 0)
	{
		netMsg->msg_Code[0] = 't';
		netMsg->msg_Code[1] = 'm';
		netMsg->msg_Code[2] = 'o';
		encodeMessage(netMsg, mensaje);
	}
}

void AlanTuring::encodeMessage(NetworkMessage* netMsg, const Mensaje mensaje)
{
	DataMessage dataMsg;

	dataMsg.msg_status = '-';
	string id(mensaje.id.c_str());
	id.append("$");

	//copia el ID extraido del xml en el struct DataMessage
	bzero(dataMsg.msg_ID,MESSAGE_ID_BYTES_LIMIT);
	memcpy(dataMsg.msg_ID, id.c_str(), MESSAGE_ID_BYTES_LIMIT);

	//copia el valor string al buffer de DataMeessage
	bzero(dataMsg.msg_value, MESSAGE_VALUE_SIZE);
	if (mensaje.id.length() < MESSAGE_ID_BYTES_LIMIT)
	{
		memcpy(dataMsg.msg_ID + id.length(), mensaje.valor.c_str(), MESSAGE_ID_BYTES_LIMIT - id.length());
		memcpy(dataMsg.msg_value, mensaje.valor.c_str() + MESSAGE_ID_BYTES_LIMIT - id.length(), MESSAGE_VALUE_SIZE);
	}
	else
		memcpy(dataMsg.msg_value, mensaje.valor.c_str(), MESSAGE_VALUE_SIZE);

	//network message length
	netMsg->msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES + 1 +id.length() + mensaje.valor.length();

	//network message data
	bzero(netMsg->msg_Data, MESSAGE_DATA_SIZE);
	memcpy(netMsg->msg_Data, &dataMsg, MESSAGE_DATA_SIZE);

}

void AlanTuring::changeDataValue(NetworkMessage* networkMessage, std::string& newValue)
{
	DataMessage dataMessage =  decodeMessage(*networkMessage);
	//memcpy(&dataMessage, networkMessage->msg_Data, sizeof(DataMessage));

	string id(dataMessage.msg_ID);
	id.append("$");

	bzero(dataMessage.msg_ID,MESSAGE_ID_BYTES_LIMIT);
	memcpy(dataMessage.msg_ID, id.c_str(), MESSAGE_ID_BYTES_LIMIT);

	bzero(dataMessage.msg_value, MESSAGE_VALUE_SIZE);
	if (id.length() < MESSAGE_ID_BYTES_LIMIT)
	{
		memcpy(dataMessage.msg_ID + id.length(), newValue.c_str(), MESSAGE_ID_BYTES_LIMIT - id.length());
		memcpy(dataMessage.msg_value, newValue.c_str() + MESSAGE_ID_BYTES_LIMIT - id.length(), MESSAGE_VALUE_SIZE);
	}
	else
		memcpy(dataMessage.msg_value, newValue.c_str(), MESSAGE_VALUE_SIZE);

	networkMessage->msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES + 1 +id.length() + newValue.length();

	//network message data
	bzero(networkMessage->msg_Data, MESSAGE_DATA_SIZE);
	memcpy(networkMessage->msg_Data, &dataMessage, MESSAGE_DATA_SIZE);
}


void AlanTuring::setNetworkMessageStatus(NetworkMessage* networkMessage, char statusCode)
{
	DataMessage dataMessage;
	memcpy(&dataMessage, networkMessage->msg_Data, sizeof(DataMessage));
	dataMessage.msg_status = statusCode;
	bzero(networkMessage->msg_Data, MESSAGE_DATA_SIZE);
	memcpy(networkMessage->msg_Data, &dataMessage, sizeof(dataMessage));

}

/*char* AlanTuring::setNetworkMessageStatus(NetworkMessage* networkMessage, char statusCode)
{
	char bufferSalida[MESSAGE_BUFFER_SIZE];
	bzero(bufferSalida, MESSAGE_BUFFER_SIZE);

	DataMessage dataMessage = decodeMessage(networkMessage->msg_Data);
	dataMessage.msg_status = statusCode;

	bzero(networkMessage->msg_Data, MESSAGE_DATA_SIZE);
	memcpy(networkMessage->msg_Data, &dataMessage, sizeof(dataMessage));

	//copia el mensaje de red al buffer de salida
	memcpy(bufferSalida, networkMessage, sizeof(NetworkMessage));

	return bufferSalida;
}*/

/*intMessage AlanTuring::decodeIntMessage(char* datosCodigoEnigma)
{
	intMessage intMsg;
	memcpy(&intMsg, datosCodigoEnigma, sizeof(intMessage));
	return intMsg;
}

charMessage AlanTuring::decodeCharMessage(char* datosCodigoEnigma)
{
	charMessage charMsg;
	memcpy(&charMsg, datosCodigoEnigma, sizeof(charMessage));
	return charMsg;
}

doubleMessage AlanTuring::decodeDoubleMessage(char* datosCodigoEnigma)
{
	doubleMessage doubleMsg;
	memcpy(&doubleMsg, datosCodigoEnigma, sizeof(doubleMessage));
	return doubleMsg;
}

stringMessage AlanTuring::decodeStringMessage(char* datosCodigoEnigma)
{
	stringMessage stringMsg;
	memcpy(&stringMsg, datosCodigoEnigma, sizeof(stringMessage));
	return stringMsg;
}*/

/*void AlanTuring::encodeIntMessage(NetworkMessage* netMsg, const Mensaje mensaje)
{
	intMessage intMsg;
	//network message code
	netMsg->msg_Code[0] = 'i';
	netMsg->msg_Code[1] = 'n';
	netMsg->msg_Code[2] = 't';

	//copia el ID extraido del xml en un buffer y luego al struct intMessage
	char buffer[MESSAGE_ID_BYTES_LIMIT];
	bzero(buffer,MESSAGE_ID_BYTES_LIMIT);
	strncpy(buffer, mensaje.id.c_str(), sizeof(buffer));
	//copia id al msg_ID
	strncpy(intMsg.msg_ID, buffer, sizeof(buffer));

	intMsg.msg_value = std::stoi (mensaje.valor);

	//network message length
	netMsg->msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES + (unsigned short)sizeof(intMessage);

	//network message data
	char dataBuffer[MESSAGE_DATA_SIZE];
	bzero(dataBuffer,MESSAGE_DATA_SIZE);
	memcpy(netMsg->msg_Data, &intMsg, sizeof(intMessage));
}

void AlanTuring::encodeCharMessage(NetworkMessage* netMsg, const Mensaje mensaje)
{
	charMessage charMsg;
	//network message code
	netMsg->msg_Code[0] = 'c';
	netMsg->msg_Code[1] = 'h';
	netMsg->msg_Code[2] = 'r';

	//copia el ID extraido del xml en un buffer y luego al struct intMessage
	char buffer[MESSAGE_ID_BYTES_LIMIT];
	bzero(buffer,MESSAGE_ID_BYTES_LIMIT);
	strncpy(buffer, mensaje.id.c_str(), sizeof(buffer));
	//copia id al msg_ID
	strncpy(charMsg.msg_ID, buffer, sizeof(buffer));

	if (mensaje.valor.length() > 1)
		Logger::Instance()->LOG("Mensaje: Mensaje del tipo char contiene multiplos caracteres. Se enviará solo el primero.", WARN);
	charMsg.msg_value = mensaje.valor.at(0);;

	//network message length
	netMsg->msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES + (unsigned short)sizeof(charMessage);

	//network message data
	char dataBuffer[MESSAGE_DATA_SIZE];
	bzero(dataBuffer,MESSAGE_DATA_SIZE);
	memcpy(netMsg->msg_Data, &charMsg, sizeof(charMessage));
}

void AlanTuring::encodeDoubleMessage(NetworkMessage* netMsg, const Mensaje mensaje)
{
	doubleMessage doubleMsg;
	//network message code
	netMsg->msg_Code[0] = 'd';
	netMsg->msg_Code[1] = 'b';
	netMsg->msg_Code[2] = 'l';

	//copia el ID extraido del xml en un buffer y luego al struct intMessage
	char buffer[MESSAGE_ID_BYTES_LIMIT];
	bzero(buffer,MESSAGE_ID_BYTES_LIMIT);
	strncpy(buffer, mensaje.id.c_str(), sizeof(buffer));
	//copia id al msg_ID
	strncpy(doubleMsg.msg_ID, buffer, sizeof(buffer));

	doubleMsg.msg_value = std::stod (mensaje.valor);

	//network message length
	netMsg->msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES + (unsigned short)sizeof(doubleMessage);

	//network message data
	char dataBuffer[MESSAGE_DATA_SIZE];
	bzero(dataBuffer,MESSAGE_DATA_SIZE);
	memcpy(netMsg->msg_Data, &doubleMsg, sizeof(doubleMessage));
}

void AlanTuring::encodeStringMessage(NetworkMessage* netMsg, const Mensaje mensaje)
{
	stringMessage stringMsg;
	//network message code
	netMsg->msg_Code[0] = 's';
	netMsg->msg_Code[1] = 't';
	netMsg->msg_Code[2] = 'r';

	//copia el ID extraido del xml en un buffer y luego al struct intMessage
	char buffer[MESSAGE_ID_BYTES_LIMIT];
	bzero(buffer,MESSAGE_ID_BYTES_LIMIT);
	strncpy(buffer, mensaje.id.c_str(), sizeof(buffer));
	//copia id al msg_ID
	strncpy(stringMsg.msg_ID, buffer, sizeof(buffer));

	stringMsg.msg_value = mensaje.valor.c_str();

	//network message length
	netMsg->msg_Length = MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES + (unsigned short)sizeof(stringMessage);

	//network message data
	char dataBuffer[MESSAGE_DATA_SIZE];
	bzero(dataBuffer,MESSAGE_DATA_SIZE);
	memcpy(netMsg->msg_Data, &stringMsg, sizeof(stringMessage));
}*/
