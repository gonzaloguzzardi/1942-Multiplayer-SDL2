/*
 * PopUp.cpp
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#include "PopUp.h"
#include "../Singletons/GameTimeHelper.h"
#include "../Utils/Random.h"
#include "../Game.h"

PopUp::PopUp():MoveableObject(),
			   m_kDissappearanceTime(500),
			   m_ownerID(0),
			   m_remainingTime(500),
			   m_dead(false)
{
	float speed = Random::getFloatRange(0.75f, 1.5f);
	m_speed.m_x = speed;
	m_speed.m_y = speed;

	m_direction.m_x = Random::getRange(-1.0f, 1.0f);
	m_direction.m_y = Random::getRange(-1.0f, 1.0f);

	m_textureID = 104;
	m_layer = FOREGROUND;
}

PopUp::PopUp(int ownerID):MoveableObject(),
			   m_kDissappearanceTime(500),
			   m_ownerID(ownerID),
			   m_remainingTime(500),
			   m_dead(false)
{
	float speed = Random::getFloatRange(0.75f, 1.5f);
	m_speed.m_x = speed;
	m_speed.m_y = speed;

	m_direction.m_x = Random::getRange(-1.0f, 1.0f);
	m_direction.m_y = Random::getRange(-1.0f, 1.0f);
}

PopUp::~PopUp()
{

}

void PopUp::update()
{
	animatePopUp();
	MoveableObject::update();
	if (m_dirty)
	{
		sendDrawMessage(!m_dead);
		m_dirty = false;
	}
}

void PopUp::animatePopUp()
{
	m_remainingTime -= GameTimeHelper::Instance()->deltaTime();
	//m_alpha = 500 * m_remainingTime/m_kDissappearanceTime;
	if (m_remainingTime <= 0)
	{
		m_dead = true;
		m_dirty = true;
	}
}

void PopUp::sendDrawMessage(bool isAlive)
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
