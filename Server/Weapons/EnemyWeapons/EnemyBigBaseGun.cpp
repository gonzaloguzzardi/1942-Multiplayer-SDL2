/*
 * EnemyBaseGun.cpp
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#include "EnemyBigBaseGun.h"
#include "CircleBullet.h"
#include "../Bullet.h"
#include "../BulletsHandler.h"
#include "../../Singletons/CollisionHandler.h"

EnemyBigBaseGun::EnemyBigBaseGun(): Weapon()
{
	m_shootCooldown = 150;
	m_waitTime = 0;
	m_canShoot = true;
	m_bulletSpeed = 10;
}

EnemyBigBaseGun::~EnemyBigBaseGun()
{
}

bool EnemyBigBaseGun::shoot(const Vector2D& bulletPosition, const Vector2D& direction)
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

		Vector2D dir2;
		std::shared_ptr<Bullet> bullet1 = std::make_shared<CircleBullet>();
		bullet1->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		dir2.m_x = direction.m_x * 0.92718385456 - direction.m_y * 0.37460659341;
		dir2.m_y = direction.m_x * 0.37460659341 + direction.m_y * 0.92718385456;
		bullet1->setDirection(dir2);
		bullet1->setSpeed(Vector2D(m_bulletSpeed, m_bulletSpeed));
		bullet1->setOwnerID(m_ownerID);
		bullet1->setOwnerTeam(m_ownerTeamNumber);

		BulletsHandler::Instance()->addBullet(bullet1);

		CollitionHandler::Instance()->addEnemyBullet(bullet1);

		std::shared_ptr<Bullet> bullet2 = std::make_shared<CircleBullet>();
		bullet2->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		dir2.m_x = direction.m_x * 0.92718385456 + direction.m_y * 0.37460659341;
		dir2.m_y = direction.m_x * -0.37460659341 + direction.m_y * 0.92718385456;
		bullet2->setDirection(dir2);
		bullet2->setSpeed(Vector2D(m_bulletSpeed, m_bulletSpeed));
		bullet2->setOwnerID(m_ownerID);
		bullet2->setOwnerTeam(m_ownerTeamNumber);

		BulletsHandler::Instance()->addBullet(bullet2);

		CollitionHandler::Instance()->addEnemyBullet(bullet2);


		m_waitTime = 0;
		m_canShoot = false;
		didShoot = true;
	}

	return didShoot;

}

