/*
 * Weapon.h
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_WEAPON_H_
#define WEAPONS_WEAPON_H_

#include "Bullet.h"
#include "../Singletons/GameTimeHelper.h"
#include "../Vector2D.h"
#include <vector>
#include <memory>

//Abstacta
class Weapon
{
public:
    virtual ~Weapon() {}

    //devuelve true si se realizo el disparo.
	virtual bool shoot(const Vector2D& bulletPosition, const Vector2D& direction) = 0;

	virtual void update()
	{
		if (!m_canShoot)
		{
			//actualiza el timer solo si canShoot es falso, para evitar entrar innecesariamente
			m_waitTime += GameTimeHelper::Instance()->deltaTime();
			//printf("waitTime del arma: %d \n", GameTimeHelper::Instance()->deltaTime());
			if (m_waitTime >= m_shootCooldown)
				m_canShoot = true;
		}
	}

    void setBulletSpeed(int speed) { m_bulletSpeed = speed; }
    void setShootingCooldown(int cooldown) { m_shootCooldown = cooldown; }

    void setOwnerID(int ID) { m_ownerID = ID; }
    void setOwnerTeam(int teamNumber) { m_ownerTeamNumber = teamNumber; }
    int getBulletSpeed(){ return m_bulletSpeed; }

protected:

	Weapon(): m_shootCooldown(0.5f),
			  m_waitTime(0),
			  m_canShoot(true),
			  m_ownerID(0),
			  m_ownerTeamNumber(0),
			  m_bulletSpeed(10)
	{}

	//Cooldown de disparo en segundos
	int m_shootCooldown;
	int m_waitTime;
	bool m_canShoot;

	int m_ownerID;
	int m_ownerTeamNumber;

	//Por Obligacion
	int m_bulletSpeed;

};

#endif /* WEAPONS_WEAPON_H_ */
