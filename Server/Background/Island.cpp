/*
 * Island.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: gonzalo
 */

#include "Island.h"

Uint32 Island_TimerCallback(Uint32 interval, void *island) {
   ((class Island *)island)->reappear();
   return 0;
}

Island::Island() : RecurrentObject()
{
	m_tag = "island";
	m_layer = BACKGROUND;
}

Island::~Island()
{
}

void Island::load(int x, int y, int width, int height, int textureID, int numFrames)
{
    // posicion de la nave en el juego. Puede estar fuera de camara y se dibujará aunque no sea visible
    m_position = Vector2D(x, y);

    // Informacion del sprite/spritesheet
    m_width = width;
    m_height = height;
    m_textureID = textureID;
    m_numFrames = numFrames;

    TextureManager::Instance()->load("Assets/Sprites/island.jpg", m_textureID, Game::Instance()->getRenderer());
}

void Island::draw()
{
    TextureManager::Instance()->drawFrame(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height,
    										m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), m_angle, m_alpha);
}

void Island::update()
{
	/*if (!m_waiting)
	{
		m_dirty = true;
		//se mueve en la dirección seteada
		m_position.m_x += (m_direction.getX() * m_scrollSpeed.getX());
		m_position.m_y += (m_direction.getY() * m_scrollSpeed.getY());

		if (m_position.getY() >= Game::Instance()->getGameHeight())
		{
			//printf("El objeto %s %f %d scrolleo toda la pantalla.\n", m_tag.c_str(), m_position.getY(), Game::Instance()->getGameHeight());
			if (m_reappearanceTime > 0)
			{
				//inicia Timer con el callback de reaparecer, luego del tiempo establecido
				timerID = SDL_AddTimer( m_reappearanceTime, Island_TimerCallback, this);
				m_waiting = true;
			}
			else
			{
				//reaparece inmediatamente sin espera
				reappear();
			}
		}
	}*/

	//if (m_dirty)
	//{
		sendDrawMessage(!m_obsolet);
		//m_dirty = false;
	//}
}

void Island::reappear()
{
	//setea Y en el comienzo
	m_position.setY(0 - m_height);

	//setea X en una posicion aleatoria
	int randomX = Random::getRange(0, Game::Instance()->getGameWidth() - m_width);
	m_position.setX(randomX);
	m_waiting = false;
}

void Island::clean()
{
	//setea Y en el comienzo
	RecurrentObject::clean();
	//sendDrawMessage(false);
}

void Island::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.unused1 = false;
	drawMsg.connectionStatus = true;
	drawMsg.alive = isAlive;
	drawMsg.hasSound = false;

	if (!isAlive)
	{
		m_canRecycle = true;
	}

	drawMsg.objectID = m_objectId;
	drawMsg.layer = m_layer;
	drawMsg.soundID = 0;
	drawMsg.column = m_currentFrame;
	drawMsg.row = m_currentRow;
	drawMsg.posX = m_position.getX();
	drawMsg.posY = m_position.getY();
	drawMsg.textureID = m_textureID;
	drawMsg.angle = static_cast<float>(m_angle);
	drawMsg.alpha = m_alpha;
	drawMsg.vacio = 0;

	if (USE_DRAWMESSAGE_PACKAGING)
	{
		Game::Instance()->addToPackage(drawMsg);
	}
	else
	{
		Game::Instance()->sendToAllClients(drawMsg);
	}
}

