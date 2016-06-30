/*
 * BackgroundObject.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: gonzalo
 */

#include "RecurrentObject.h"

//Callback, no puede ser miembro de la función. Puede si es estática...tal vez...
Uint32 BackgroundObject_TimerCallback(Uint32 interval, void *backgroundObject) {
   ((class RecurrentObject *)backgroundObject)->reappear();
   return 0;
}

RecurrentObject::RecurrentObject() :  GameObject(),
				m_direction(0,1),
				m_scrollSpeed(0,2),
				m_reappearanceTime(0),
				m_waiting(false),
				timerID(0)
{
	m_tag = "BackgroundObject";
	m_layer = MIDDLEGROUND;
}

RecurrentObject::RecurrentObject(const Vector2D& direction, const Vector2D& scrollSpeed) :  GameObject(),
																							m_reappearanceTime(0),
																							m_waiting(false),
																							timerID(0)
{
	m_tag = "BackgroundObject";
	m_direction = direction;
	m_scrollSpeed = scrollSpeed;
}


void RecurrentObject::load(int x, int y, int width, int height, int textureID, int numFrames)
{
    // posicion de la nave en el juego. Puede estar fuera de camara y se dibujará aunque no sea visible
    m_position = Vector2D(x, y);

    // Informacion del sprite/spritesheet
    m_width = width;
    m_height = height;
    m_textureID = textureID;
    m_numFrames = numFrames;
    m_hitRadius = m_width/2;
}

void RecurrentObject::draw()
{
    /*TextureManager::Instance()->drawFrame(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height,
    										m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), m_angle, m_alpha);*/
}

void RecurrentObject::update()
{
	/*if (!m_waiting)
	{
		//se mueve en la dirección seteada
		m_position.m_x += (m_direction.getX() * m_scrollSpeed.getX());
		m_position.m_y += (m_direction.getY() * m_scrollSpeed.getY());

		if (m_position.getY() >= Game::Instance()->getGameHeight())
		{
			printf("El objeto %s %f %d scrolleo toda la pantalla.\n", m_tag.c_str(), m_position.getY(), Game::Instance()->getGameHeight());
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
}


void RecurrentObject::reappear()
{
	//setea Y en el comienzo
	m_position.setY(0 - m_height);

	//setea X en una posicion aleatoria
	int randomX = Random::getRange(0, 600 - m_width);
	//int randomX = Random::getRange(0, Game::Instance()->getGameWidth() - m_width);
	m_position.setX(randomX);
}

void RecurrentObject::clean()
{
	if (timerID != 0)
		SDL_RemoveTimer( timerID );
}
