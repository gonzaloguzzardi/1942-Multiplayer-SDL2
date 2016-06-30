/*
 * PowerUp.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#include "PowerUp.h"
#include "../Game.h"
#include "../Player.h"

PowerUp::PowerUp(): MoveableObject(),
		m_pickedUp(false),
		m_animatingPickUp(false),
		m_dead(false),
		m_appearing(true),
		m_dissappearing(false),
		m_savedTextureID(70),
		m_savedNumFrames(1),
		m_appearenceAnimationTime(1000),
		m_appearenceRemainingTime(1000),
	    m_disappearenceRemainingTime(5000)
{
	m_tag = "PowerUp";
}

PowerUp::~PowerUp()
{

}

void PowerUp::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	MoveableObject::load(x, y, width, height, textureID, numFrames);
	doAppearAnimation();
}

void PowerUp::draw()
{
	//MoveableObject::draw();
}
void PowerUp::update()
{
	if (m_appearing)
	{
		updateAppearingAnimation();
	}

	if (m_dissappearing)
	{
		updateDissapeareanceTimer();
	}

	if ((!m_pickedUp) && (!m_dead))
	{
		MoveableObject::update();
	}
	if (m_dirty)
		sendDrawMessage(!m_dead);
}

void PowerUp::clean()
{
	MoveableObject::clean();
}


bool PowerUp::isVisibleFromCamera()
{
	if ((m_position.m_x - m_width < 0) && (m_position.m_x > Game::Instance()->getGameWidth()) &&
		(m_position.m_y - m_height < 0) && (m_position.m_y > Game::Instance()->getGameHeight()))
	{
		return false;
	}

	return true;
}


bool PowerUp::canPickUp()
{
	return (!m_pickedUp && !m_appearing);
}

void PowerUp::doAppearAnimation()
{
	m_appearing = true;
	m_savedTextureID = m_textureID;
	m_savedNumFrames = m_numFrames;
	m_textureID = 90;
	m_numFrames = 5;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_appearenceRemainingTime = m_appearenceAnimationTime;
}

void PowerUp::startDissapeareanceTimer()
{
	m_dissappearing = true;
	m_disappearenceRemainingTime = 5000;
}

void PowerUp::updateDissapeareanceTimer()
{
	m_disappearenceRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	if (m_disappearenceRemainingTime <= 0)
	{
		m_pickedUp = true;
		m_dead = true;
	}
}

void PowerUp::updateAppearingAnimation()
{
	m_appearenceRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_appearenceAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	int lastRow = m_currentRow;

	m_currentFrame = ((m_appearenceAnimationTime - m_appearenceRemainingTime) / step);
	//m_currentRow = ((m_appearenceAnimationTime - m_appearenceRemainingTime) / step) / 5;

	if (m_appearenceRemainingTime <= 0)
	{
		m_appearing = false;
		m_textureID = m_savedTextureID;
		m_currentFrame = 0;
		m_currentRow = 0;
		m_numFrames = m_savedNumFrames;
		m_dirty = true;
		startDissapeareanceTimer();
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}


void PowerUp::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.unused1 = false;
	drawMsg.connectionStatus = true;
	drawMsg.alive = isAlive;

	if(m_soundDirty)
	{
		drawMsg.hasSound = true;
		m_soundDirty = false;

	}else
	{
		drawMsg.hasSound = false;
	}

	drawMsg.objectID = m_objectId;
	drawMsg.layer = m_layer;
	drawMsg.soundID = m_soundSendId;
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
