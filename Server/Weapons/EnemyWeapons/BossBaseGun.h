/*
 * EnemyBaseGun.h
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_ENEMYWEAPONS_BOSSBASEGUN_H_
#define WEAPONS_ENEMYWEAPONS_BOSSBASEGUN_H_

#include "../Weapon.h"
#include "../../Vector2D.h"
#include "../../Singletons/GameTimeHelper.h"

class BossBaseGun : public Weapon
{
public:
	BossBaseGun();
	virtual ~BossBaseGun();

	virtual bool shoot(const Vector2D& bulletPosition, const Vector2D& direction);

};



#endif /* WEAPONS_ENEMYWEAPONS_BOSSBASEGUN_H_ */
