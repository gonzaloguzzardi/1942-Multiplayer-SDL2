/*
 * Background.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: gonzalo
 */

#include "Background.h"

Background::Background() : GameObject(),
							m_direction(0,0),
							m_scrollSpeed(0,0)
{
	m_layer = BACKGROUND;
}

Background::Background(const Vector2D& direction, const Vector2D& scrollSpeed): GameObject()
{
	m_direction = direction;
	m_scrollSpeed = scrollSpeed;
}

void Background::load(int x, int y, int width, int height, int textureID)
{
    // posicion de la nave en el juego. Puede estar fuera de camara y se dibujará aunque no sea visible
    m_position = Vector2D(x, y);
    m_numFrames = 1;
    // Informacion del sprite/spritesheet
    m_width = width;
    m_height = height;
    m_textureID = textureID;

    //esta parte podria ir en un background hijo
    TextureManager::Instance()->load("Assets/Sprites/water.jpg", m_textureID, Game::Instance()->getRenderer());
}

Background::~Background()
{
}


void Background::draw()
{
    TextureManager::Instance()->drawFrame(m_textureID, 0, 0, m_width, m_height, 0, 0, Game::Instance()->getRenderer(), 0, 255);
}

void Background::update()
{
	//se mueve en la dirección seteada
	if (m_direction.getX() != 0)
	{
		m_position.m_x += (m_direction.getX() * m_scrollSpeed.getX());
		m_dirty = true;
	}

	if (m_direction.getY() != 0)
	{
		m_position.m_y += (m_direction.getY() * m_scrollSpeed.getY());
		m_dirty = true;
	}

	if (m_dirty)
	{
		sendDrawMessage(true);
		m_dirty = false;
	}
}

void Background::clean()
{

}

void Background::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.unused1 = false;
	drawMsg.connectionStatus = true;
	drawMsg.alive = isAlive;
	drawMsg.hasSound = false;

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
