/*
 * DrawMessagesPacker.cpp
 *
 *  Created on: May 8, 2016
 *      Author: gonzalo
 */

#include "DrawMessagesPacker.h"

DrawMessagesPacker::DrawMessagesPacker(server* server): m_packageSize(DRAW_MESSAGE_PACK_SIZE)
{
	m_serverInstance = server;
}

void DrawMessagesPacker::addDrawMessage(DrawMessage drawMessage)
{
	m_drawMessages.push_back(drawMessage);
}

void DrawMessagesPacker::sendPackedMessages()
{
	if (m_drawMessages.empty())
			return;

	int messagesAmount = m_drawMessages.size();

	int completePackages = messagesAmount / m_packageSize;
	int lastPackageMessages = messagesAmount % m_packageSize;

	//arma los paquetes completos
	for (int i = 0; i < completePackages; i++)
	{
		DrawMessagePack pack;
		pack.totalSize = (sizeof(DrawMessage) * m_packageSize) + sizeof(pack.totalSize);
		for (int currentMessage = m_packageSize * i; currentMessage < m_packageSize * (i + 1); currentMessage++)
		{
			pack.drawMessages[currentMessage % m_packageSize] = m_drawMessages.at(currentMessage);
		}
		m_serverInstance->sendPackToAll(pack);

	}
	//arma el ultimo paquete
	DrawMessagePack lastPack;
	for (int currentMessage = m_packageSize * completePackages; currentMessage < messagesAmount; currentMessage++)
	{
		lastPack.totalSize = (sizeof(DrawMessage) * lastPackageMessages) + sizeof(lastPack.totalSize);
		lastPack.drawMessages[currentMessage % m_packageSize] = m_drawMessages.at(currentMessage);
	}
	m_serverInstance->sendPackToAll(lastPack);

	m_drawMessages.clear();
}

void DrawMessagesPacker::clean()
{
	m_drawMessages.clear();
}
