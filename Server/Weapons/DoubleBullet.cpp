/*
 * DoubleBullet.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#include "DoubleBullet.h"

DoubleBullet::DoubleBullet(): Bullet()

{
	m_tag = "DoubleBullet";
	m_layer = MIDDLEGROUND;
	m_textureID = 52;

	m_dyingTime = 3000;
	m_speed = Vector2D(10,10);
	m_damage = 200;
	m_width = 48;
}

DoubleBullet::~DoubleBullet() {}


void DoubleBullet::update()
{
	Bullet::update();
}
