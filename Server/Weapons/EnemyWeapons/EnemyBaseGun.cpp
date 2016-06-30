/*
 * EnemyBaseGun.cpp
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#include "EnemyBaseGun.h"
#include "CircleBullet.h"
#include "../Bullet.h"
#include "../BulletsHandler.h"
#include "../../Singletons/CollisionHandler.h"

EnemyBaseGun::EnemyBaseGun(): Weapon()
{
	m_shootCooldown = 150;
	m_waitTime = 0;
	m_canShoot = true;
	m_bulletSpeed = 10;
}

EnemyBaseGun::~EnemyBaseGun()
{
}

bool EnemyBaseGun::shoot(const Vector2D& bulletPosition, const Vector2D& direction)
{
	bool didShoot = false;

	if (m_canShoot)
	{
		std::shared_ptr<Bullet> bullet = std::make_shared<CircleBullet>();
		bullet->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		bullet->setDirection(direction);
		bullet->setSpeed(Vector2D(m_bulletSpeed, m_bulletSpeed));
		bullet->setOwnerID(m_ownerID);
		bullet->setOwnerTeam(m_ownerTeamNumber);

		BulletsHandler::Instance()->addBullet(bullet);

		CollitionHandler::Instance()->addEnemyBullet(bullet);

		m_waitTime = 0;
		m_canShoot = false;
		didShoot = true;
	}

	return didShoot;

}

