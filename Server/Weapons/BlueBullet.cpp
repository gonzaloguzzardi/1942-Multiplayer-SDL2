/*
 * BlueBullet.cpp
 *
 *  Created on: May 9, 2016
 *      Author: gonzalo
 */

#include "BlueBullet.h"

BlueBullet::BlueBullet(): Bullet()

{
	m_tag = "BlueBullet";
	m_layer = MIDDLEGROUND;
	m_textureID = 50;

	m_dyingTime = 3000;
	m_speed = Vector2D(10,10);
	action = false;
}

BlueBullet::~BlueBullet() {}


void BlueBullet::update()
{
	if(!m_dead)
	{
		MoveableObject::update();
		if((!action) && (m_dyingTime <= 2000 )){
			action = true;
			m_direction.setX(m_direction.getX()* -1);
		}

		m_dyingTime -= GameTimeHelper::Instance()->deltaTime();
		if (m_dyingTime <= 0)
		{
			m_dead = true;
		}
	}
	else
	{
		m_direction.setX(0);
		m_direction.setY(0);
	}

	if (m_dirty)
	{
		sendDrawMessage(true);
		m_dirty = false;
	}

}
