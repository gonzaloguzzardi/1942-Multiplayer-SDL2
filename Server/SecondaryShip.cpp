/*
 * SecondaryShip.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: gonzalo
 */

#include "SecondaryShip.h"
#include "Game.h"
#include "Vector2D.h"
#include "Weapons/Bullet.h"
#include "Weapons/BulletsHandler.h"
#include "Singletons/CollisionHandler.h"

SecondaryShip::SecondaryShip(): MoveableObject(),
								m_health(0),
								m_dead(false),
								m_canRecycle(false)
{
	m_textureID = 73;
	m_tag = "Nave Secundaria";
	m_layer = FOREGROUND;

	m_direction.m_x = 0;
	m_direction.m_y = 0;
}

SecondaryShip::~SecondaryShip()
{

}

void SecondaryShip::damage(int damage)
{
	m_health -= damage;
	if (m_health <= 0)
	{
		m_dead = true;
	}
}

void SecondaryShip::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	MoveableObject::load(x, y, width, height, textureID, numFrames);
	m_hitRadius = m_width;
}

void SecondaryShip::shoot(const Vector2D& bulletPosition, const Vector2D& direction, int speed, int ownerID, int teamID)
{
	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
	bullet->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
	bullet->setDirection(direction);
	bullet->setSpeed(Vector2D(speed, speed));
	bullet->setOwnerID(ownerID);
	bullet->setOwnerTeam(teamID);

	BulletsHandler::Instance()->addBullet(bullet);
	CollitionHandler::Instance()->addPlayerBullet(bullet);
}

void SecondaryShip::update()
{
	sendDrawMessage(!m_dead);
}


void SecondaryShip::sendDrawMessage(bool isAlive)
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
