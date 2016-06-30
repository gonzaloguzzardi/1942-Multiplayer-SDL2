/*
 * DualWeapon.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#include "DualWeapon.h"
#include "../Bullet.h"
#include "../DoubleBullet.h"
#include "../BulletsHandler.h"
#include "../../Singletons/CollisionHandler.h"

DualWeapon::DualWeapon(): Weapon()
{
	m_shootCooldown = 150;
	m_waitTime = 0;
	m_canShoot = true;
	m_bulletSpeed = 10;
}

DualWeapon::~DualWeapon()
{
}

bool DualWeapon::shoot(const Vector2D& bulletPosition, const Vector2D& direction)
{
	bool didShoot = false;

	if (m_canShoot)
	{
		std::shared_ptr<Bullet> bullet = std::make_shared<DoubleBullet>();
		bullet->setPosition(Vector2D(bulletPosition.m_x - (bullet->getWidth()/8), bulletPosition.m_y));
		bullet->setDirection(direction);
		bullet->setSpeed(Vector2D(m_bulletSpeed, m_bulletSpeed));
		bullet->setOwnerID(m_ownerID);
		bullet->setOwnerTeam(m_ownerTeamNumber);

		BulletsHandler::Instance()->addBullet(bullet);

		CollitionHandler::Instance()->addPlayerBullet(bullet);

		m_waitTime = 0;
		m_canShoot = false;
		didShoot = true;
	}

	return didShoot;

}

