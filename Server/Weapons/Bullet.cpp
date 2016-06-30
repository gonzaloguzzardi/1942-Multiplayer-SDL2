/*
 * Bullet.cpp
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#include "Bullet.h"
#include "../Game.h"

Bullet::Bullet(): MoveableObject(),
			m_dead(false),
			m_exploting(false),
		    m_explotionAnimationTime(750),
		    m_explotionRemainingTime(0),
			m_damage(100),
			m_ownerID(0),
			m_ownerTeamNumber(0)
{
	m_tag = "Bullet";
	m_layer = MIDDLEGROUND;
	m_textureID = 50;

	m_dyingTime = 3000;
	m_speed = Vector2D(20,20);

}

Bullet::~Bullet() {}


void Bullet::update()
{
	if (m_exploting)
	{
		updateExplotionAnimation();
	}

	if((!m_dead) && (!m_exploting))
	{
		MoveableObject::update();
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

bool Bullet::isDead()
{
	return m_dead;
}

void Bullet::kill()
{
	if (EXPLOTE)
	{
		explote();
	}
	else
	{
		m_dead = true;
	}

}

void Bullet::explote()
{
	m_position.m_y -= 10;
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	m_numFrames = 6;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 45;
}

void Bullet::updateExplotionAnimation()
{
	m_explotionRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_explotionAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	int lastRow = m_currentRow;

	m_currentFrame = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) % 6;
	m_currentRow = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) / 6;

	if (m_explotionRemainingTime <= 0)
	{
		m_dead = true;
		m_exploting = false;
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}

void Bullet::clean()
{
	MoveableObject::clean();
	sendDrawMessage(false);
}

void Bullet::sendDrawMessage(bool isAlive)
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
	drawMsg.angle = m_angle;
	drawMsg.alpha = m_alpha;

	if (!isAlive)
		m_canRecycle = true;

	if (USE_DRAWMESSAGE_PACKAGING)
	{
		Game::Instance()->addToPackage(drawMsg);
	}
	else
	{
		Game::Instance()->sendToAllClients(drawMsg);
	}
}
