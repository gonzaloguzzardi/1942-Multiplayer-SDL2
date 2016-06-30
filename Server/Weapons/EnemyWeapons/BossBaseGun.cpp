#include "BossBaseGun.h"
#include "CircleBullet.h"
#include "../Bullet.h"
#include "../BulletsHandler.h"
#include "../../Singletons/CollisionHandler.h"

BossBaseGun::BossBaseGun(): Weapon()
{
	m_shootCooldown = 150;
	m_waitTime = 0;
	m_canShoot = true;
	m_bulletSpeed = 10;
}

BossBaseGun::~BossBaseGun()
{
}

bool BossBaseGun::shoot(const Vector2D& bulletPosition, const Vector2D& direction)
{
	bool didShoot = false;
	Vector2D dir;
	dir.setY(0);
	dir.setX(0);
	if (m_canShoot)
	{
		for(int i=0;i<36;i++){
			std::shared_ptr<Bullet> bullet = std::make_shared<CircleBullet>();
			bullet->setPosition(Vector2D(bulletPosition.m_x, bulletPosition.m_y));
			dir.setY(sinf((360-i*10)*0.01745329251f));
			dir.setX(cosf((360-i*10)*0.01745329251f));
			bullet->setDirection(dir);
			bullet->setSpeed(Vector2D(m_bulletSpeed, m_bulletSpeed));
			bullet->setOwnerID(m_ownerID);
			bullet->setOwnerTeam(m_ownerTeamNumber);

			BulletsHandler::Instance()->addBullet(bullet);

			CollitionHandler::Instance()->addEnemyBullet(bullet);
		}

		m_waitTime = 0;
		m_canShoot = false;
		didShoot = true;
	}

	return didShoot;

}
float efficientSin(float x)
{
	float result = x;
	float squareX = x*x;
	float xPotence = squareX*x; //x^3
	result -= xPotence/6;
	xPotence = xPotence * squareX; //x^5
	result += xPotence/120;
	xPotence = xPotence * squareX; //x^7
	result -= xPotence/5040;
	return result;
}

