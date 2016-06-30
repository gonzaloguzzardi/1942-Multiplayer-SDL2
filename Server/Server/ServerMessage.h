#ifndef MENSIJE_H
#define MENSIJE_H

#include <string>
#include "../Utils/TiposDefinidos.h"

class ServerMessage
{
public:
	int clientID;
	NetworkMessage networkMessage;
};

#endif // MENSIJE_H
