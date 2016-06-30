/*
 * Background.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: gonzalo
 */

#include "Background.h"

Background::Background() : m_direction(0,0),
							m_scrollSpeed(0,0)
{
	m_layer = FOREGROUND;
}

Background::Background(const Vector2D& direction, const Vector2D& scrollSpeed)
{
	m_direction = direction;
	m_scrollSpeed = scrollSpeed;
}

void Background::load(int x, int y, int width, int height, int textureID)
{
    // posicion de la nave en el juego. Puede estar fuera de camara y se dibujará aunque no sea visible
    m_position = Vector2D(x, y);

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
		m_position.m_x += (m_direction.getX() * m_scrollSpeed.getX());

	if (m_direction.getY() != 0)
		m_position.m_y += (m_direction.getY() * m_scrollSpeed.getY());
}

void Background::clean()
{

}
