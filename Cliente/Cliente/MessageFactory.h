/*
 * MessageFactory.h
 *
 *  Created on: Apr 4, 2016
 *      Author: gonzalo
 */

#ifndef SRC_CLIENTE_MESSAGEFACTORY_H_
#define SRC_CLIENTE_MESSAGEFACTORY_H_

#include "../Utils/TiposDefinidos.h"
#include <string>

enum MsgCode
{
	msgInt,
	msgDouble,
	msgChar,
	msgString,
	msgExit,
	msgConnected,
	msgServerFull,
	msgTimeOutACK
};

class MessageFactory
{
public:

    static MessageFactory* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new MessageFactory();
        }
        return s_pInstance;
    }

	Mensaje createMessage(const std::string& msgID,const std::string& msgValue, MsgCode msgCode);

private:
	static int s_messageIDNumber;

	//Singleton
	MessageFactory();
	~MessageFactory();
    static MessageFactory* s_pInstance;
    MessageFactory(const MessageFactory&);
    MessageFactory& operator=(const MessageFactory&);
};

#endif /* SRC_CLIENTE_MESSAGEFACTORY_H_ */
