/*
 * MessagePacker.h
 *
 *  Created on: May 8, 2016
 *      Author: gonzalo
 */

#ifndef SERVER_DRAWMESSAGESPACKER_H_
#define SERVER_DRAWMESSAGESPACKER_H_


#include "../Utils/TiposDefinidos.h"
#include <vector>
#include "server.h"

class server;

class DrawMessagesPacker
{
public:
	DrawMessagesPacker(server* server);

	void sendPackedMessages();
	void addDrawMessage(DrawMessage drawMessage);

	void clean();

private:
	server* m_serverInstance;
	std::vector<DrawMessage> m_drawMessages;
	int m_packageSize;
};

#endif /* SERVER_DRAWMESSAGESPACKER_H_ */
