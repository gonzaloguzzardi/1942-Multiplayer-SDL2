/*
 * CodificadorMensajes.h
 *
 *  Created on: Apr 2, 2016
 *      Author: gonzalo
 */

#ifndef SRC_SERVER_ALANTURING_H_
#define SRC_SERVER_ALANTURING_H_

#include "../Utils/Logger.h"
#include "../Utils/TiposDefinidos.h"
#include <string>
#include <cstring>
#include <stdio.h>

/*
 * Message Codes
 *
 *int: envia nun entero
 *chr: envia un char
 *dbl: envia un double
 *str: envia un string
 *cnt: cliente conectado
 *ext: pide el cierre del receptor
 *ful: servidor lleno
 */
/*
 * Message Status
 *
 * - : no procesado
 * I : mensaje procesado e Invalido
 * V : mensaje procesado y Valido
 */

class AlanTuring
{
public:
	//ENCODES Y DECODES RELACIONADOS AL JUEGO
	int encodeDrawMessage(DrawMessage msj, char* bufferEntrada);
	int encodeDrawMessagePackage(DrawMessagePack msj, char* bufferEntrada);
	int encodeInputMessage(InputMessage msj, char* bufferEntrada);
	int encodeConnectedMessage(ConnectedMessage msj, char* bufferEntrada);
	int encodeConnectionInfoMessage(ConnectionInfo msj, char* bufferEntrada);
	int encodePlayerDisconnectionMessage(PlayerDisconnection msj, char* bufferEntrada);
	//int encodeDrawMessage(NetworkMessage* netMsg, const DrawMessage msj);
	//int encodeInputMessage(NetworkMessage* netMsg, const InputMessage msj);

	DrawMessage decodeDrawMessage(NetworkMessage netMsg);
	DrawMessagePack decodeDrawMessagePackage(NetworkMessage netMsg);
	InputMessage decodeInputMessage(NetworkMessage netMsg);
	ConnectedMessage decodeConnectedMessage(NetworkMessage netMsg);
	ConnectionInfo decodeConnectionInfoMessage(NetworkMessage netMsg);
	PlayerDisconnection decodePlayerDisconnectionMessage(NetworkMessage netMsg);
	ResetInfo decodeResetInfo(NetworkMessage netMsg);
	TextureInfo decodeTextureInfo(NetworkMessage netMsg);
	PlayerReconnectionInfo decodePlayerReconnectionInfo(NetworkMessage netMsg);
	ScoreMessage decodeScoreMessage(NetworkMessage netMsg);
	BackgroundInfo decodeBackgroundInfo(NetworkMessage netMsg);
	StageStatistics decodeStageStatistics(NetworkMessage netMsg);
	FinishGameInfo decodeFinishGameInfo(NetworkMessage netMsg);
	StageBeginning decodeStageBeginning(NetworkMessage netMsg);
	PlayerNameUpdateInfo decodePlayerNameUpdateInfo(NetworkMessage netMsg);
	PlayerDataUpdateInfo decodePlayerDataUpdateInfo(NetworkMessage netMsg);

	NetworkMessage drawMessageToNetwork(DrawMessage drawMessage);
	NetworkMessage playerDisconnectionToNetwork(PlayerDisconnection playerDiscMessage);
	NetworkMessage drawMsgPackToNetwork(DrawMessagePack drawMsgPack);
	NetworkMessage ResetMsgToNetwork(ResetInfo resetMessage);
	NetworkMessage TextureInfoToNetwork(TextureInfo textureInfoMsg);
	NetworkMessage PlayerReconnectionInfoToNetwork(PlayerReconnectionInfo playerReconnectionInfo);
	NetworkMessage ScoreMessageToNetwork(ScoreMessage scoreMessage);
	NetworkMessage BackgroundInfoToNetwork(BackgroundInfo backgroundMessage);
	NetworkMessage StageStatisticsToNetwork(StageStatistics stageStatisticsMessage);
	NetworkMessage FinishGameInfoToNetwork(FinishGameInfo finishGameInfo);
	NetworkMessage StageBeginningToNetwork(StageBeginning stageBeginningMessage);
	NetworkMessage playerNameUpdateInfoToNetwork(PlayerNameUpdateInfo playerNameUpdateInfo);
	NetworkMessage playerDataUpdateInfoToNetwork(PlayerDataUpdateInfo playerDataUpdateInfo);


	//devuelve un buffer de 256 bytes con el mensaje codificado en binario en buffeSalida y un int con el tamaño Mensaje
	int encodeXMLMessage(Mensaje mensaje, char* bufferSalida);
	int encodeNetworkMessage(NetworkMessage netMsg, char* bufferEntrada);

	//los decode reciben buffers de tamaño 256
	NetworkMessage decode (char* codigoEnigma);
	int decodeLength(char* codigoEnigma);
	DataMessage decodeMessage (NetworkMessage netMsg);

	//modificadores de NetworkMessage
	void changeDataValue(NetworkMessage* networkMessage, std::string& newValue);
	void setNetworkMessageStatus(NetworkMessage* networkMessage, char statusCode);

	/*intMessage decodeIntMessage(char* datosCodigoEnigma);
	charMessage decodeCharMessage(char* datosCodigoEnigma);
	doubleMessage decodeDoubleMessage(char* datosCodigoEnigma);
	stringMessage decodeStringMessage(char* datosCodigoEnigma);*/

	AlanTuring();
	~AlanTuring();

private:
	//llena los campos de netMsg con la información del mensaje
	void fillMsgData(NetworkMessage* netMsg, const Mensaje mensaje);

	void encodeMessage(NetworkMessage* netMsg, const Mensaje mensaje);

	/*void encodeIntMessage(NetworkMessage* netMsg, const Mensaje mensaje);
	void encodeCharMessage(NetworkMessage* netMsg, const Mensaje mensaje);
	void encodeDoubleMessage(NetworkMessage* netMsg, const Mensaje mensaje);
	void encodeStringMessage(NetworkMessage* netMsg, const Mensaje mensaje);*/
};

#endif /* SRC_SERVER_ALANTURING_H_ */
