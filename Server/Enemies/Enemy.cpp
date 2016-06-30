/*
 * Enemy.cpp
 *
 *  Created on: May 22, 2016
 *      Author: gonzalo
 */

#include "Enemy.h"
#include "../Game.h"
#include "../Weapons/Weapon.h"
#include "../Player.h"
#include "../Singletons/CollisionHandler.h"

Enemy::Enemy(): MoveableObject(),
		m_health(10),
	    m_collisionDamage(100),
		m_dead(false),
		m_dying(false),
		m_exploting(false),
		m_pointsRetrieved(false),
		m_pointOnKill(50),
		m_pointOnHit(0)
{
	m_layer = FOREGROUND;
}

Enemy::~Enemy()
{

}

void Enemy::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	 MoveableObject::load(x, y, width, height, textureID, numFrames);
}

void Enemy::draw()
{
	MoveableObject::draw();
}
void Enemy::update()
{
	if (!m_dead && !m_dying)
	{
		MoveableObject::update();
	}
	if (m_dirty)
	{
		sendDrawMessage(!m_dead);
		m_dirty = false;
	}
}
void Enemy::clean()
{
	MoveableObject::clean();
	if (m_enemyWeapon)
	{
		delete m_enemyWeapon;
	}
}

void Enemy::activateCollition()
{
	CollitionHandler::Instance()->addEnemy(this);
}

void Enemy::shoot()
{
	if (m_enemyWeapon)
	{
		m_soundDirty = true;

		m_soundSendId = 54;
		m_enemyWeapon->shoot(Vector2D(m_position.m_x + m_position.m_x/2, m_position.m_y),
							Vector2D(0, DIRECTION_DOWN));
	}
}

bool Enemy::damage(int damageReceived, bool wasShoot, Player* damager)
{
	bool killed = false;
	m_health -= damageReceived;

	if (m_health <= 0)
	{
		//Hacer explosion, setear dying en true, etc

		m_dead = true;
		killed = true;
	}
	return killed;
}

void Enemy::kill()
{
	m_health = 0;
	m_dead = true;
}

void Enemy::setWeaponStats(int shootingSpeed, int shootingCooldown, int ownerID, int teamID)
{
	if (m_enemyWeapon)
	{
		m_enemyWeapon->setBulletSpeed(shootingSpeed);
		m_enemyWeapon->setShootingCooldown(shootingCooldown);
		m_enemyWeapon->setOwnerID(ownerID);
		m_enemyWeapon->setOwnerTeam(teamID);
	}
}

bool Enemy::isVisibleFromCamera()
{
	if ((m_position.m_x - m_width < 0) && (m_position.m_x > Game::Instance()->getGameWidth()) &&
		(m_position.m_y - m_height < 0) && (m_position.m_y > Game::Instance()->getGameHeight()))
	{
		return false;
	}

	return true;
}

int Enemy::retrievePoints()
{
	m_pointsRetrieved = true;
	return m_pointOnKill;
}

bool Enemy::canRetrievePoints()
{
	return !m_pointsRetrieved;
}


void Enemy::collision()
{
	MoveableObject::collision();
}

void Enemy::sendDrawMessage(bool isAlive)
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
