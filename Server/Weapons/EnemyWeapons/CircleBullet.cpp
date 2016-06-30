/*
 * CircleBullet.cpp
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#include "CircleBullet.h"

CircleBullet::CircleBullet(): Bullet()

{
	m_tag = "CircleBullet";
	m_layer = MIDDLEGROUND;
	m_textureID = 51;

	m_dyingTime = 5000;
	m_speed = Vector2D(8, 8);
	m_damage = 100;
}

CircleBullet::~CircleBullet() {}


void CircleBullet::update()
{
	Bullet::update();
}
