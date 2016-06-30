/*
 * MessageFactory.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: gonzalo
 */

#include "MessageFactory.h"

MessageFactory* MessageFactory::s_pInstance = 0;
int MessageFactory::s_messageIDNumber = 100;

MessageFactory::MessageFactory(){}
MessageFactory::~MessageFactory(){}

Mensaje MessageFactory::createMessage(const std::string& msgID,const std::string& msgValue, MsgCode msgCode)
{
	Mensaje mensajeCreado;
	mensajeCreado.id.assign(msgID);
	mensajeCreado.id.append(std::to_string(s_messageIDNumber));
	s_messageIDNumber += 10;

	mensajeCreado.valor.assign(msgValue);
	switch(msgCode)
	{
	case msgInt:
		mensajeCreado.tipo = "int";
		break;
	case msgDouble:
		mensajeCreado.tipo = "double";
		break;
	case msgChar:
		mensajeCreado.tipo = "char";
		break;
	case msgString:
		mensajeCreado.tipo = "string";
		break;
	case msgExit:
		mensajeCreado.tipo = "exit";
		break;
	case msgConnected:
		mensajeCreado.tipo = "connected";
		break;
	case msgServerFull:
		mensajeCreado.tipo = "serverfull";
		break;
	case msgTimeOutACK:
		mensajeCreado.tipo = "timeoutACK";


	default:
		break;
	}

	return mensajeCreado;
}
