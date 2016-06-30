/*
 * FiveBulletsWeapon.cpp
 *
 *  Created on: May 9, 2016
 *      Author: gonzalo
 */

#include "FiveBulletsWeapon.h"
#include "../Bullet.h"
#include "../BlueBullet.h"
#include "../BulletsHandler.h"
#include "../../Singletons/CollisionHandler.h"

FiveBulletsWeapon::FiveBulletsWeapon(): Weapon()
{
	m_shootCooldown = 150;
	m_waitTime = 0;
	m_canShoot = true;
}

FiveBulletsWeapon::~FiveBulletsWeapon()
{
}

bool FiveBulletsWeapon::shoot(const Vector2D& bulletPosition, const Vector2D& direction)
{
	bool didShoot = false;

	if (m_canShoot)
	{
		std::shared_ptr<Bullet> bullet = std::make_shared<BlueBullet>();
		bullet->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		bullet->setDirection(Vector2D(0,DIRECTION_UP));
		bullet->setOwnerID(m_ownerID);
		bullet->setOwnerTeam(m_ownerTeamNumber);
		BulletsHandler::Instance()->addBullet(bullet);
		CollitionHandler::Instance()->addPlayerBullet(bullet);

		std::shared_ptr<Bullet> bullet2 = std::make_shared<BlueBullet>();
		bullet2->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		bullet2->setDirection(Vector2D(-0.213,DIRECTION_UP));
		bullet2->setOwnerID(m_ownerID);
		bullet2->setOwnerTeam(m_ownerTeamNumber);
		BulletsHandler::Instance()->addBullet(bullet2);
		CollitionHandler::Instance()->addPlayerBullet(bullet2);

		std::shared_ptr<Bullet> bullet3 = std::make_shared<BlueBullet>();
		bullet3->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		bullet3->setDirection(Vector2D(0.213,DIRECTION_UP));
		bullet3->setOwnerID(m_ownerID);
		bullet3->setOwnerTeam(m_ownerTeamNumber);
		BulletsHandler::Instance()->addBullet(bullet3);
		CollitionHandler::Instance()->addPlayerBullet(bullet3);

		std::shared_ptr<Bullet> bullet4 = std::make_shared<BlueBullet>();
		bullet4->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		bullet4->setDirection(Vector2D(-0.44,DIRECTION_UP));
		bullet4->setOwnerID(m_ownerID);
		bullet4->setOwnerTeam(m_ownerTeamNumber);
		BulletsHandler::Instance()->addBullet(bullet4);
		CollitionHandler::Instance()->addPlayerBullet(bullet4);


		std::shared_ptr<Bullet> bullet5 = std::make_shared<BlueBullet>();
		bullet5->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
		bullet5->setDirection(Vector2D(0.44,DIRECTION_UP));
		bullet5->setOwnerID(m_ownerID);
		bullet5->setOwnerTeam(m_ownerTeamNumber);
		BulletsHandler::Instance()->addBullet(bullet5);
		CollitionHandler::Instance()->addPlayerBullet(bullet5);


		m_waitTime = 0;
		m_canShoot = false;
		didShoot = true;
	}

	return didShoot;

}
