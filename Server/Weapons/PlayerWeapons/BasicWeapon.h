/*
 * BasicWapon.h
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_PLAYERWEAPONS_BASICWEAPON_H_
#define WEAPONS_PLAYERWEAPONS_BASICWEAPON_H_

#include "../Weapon.h"
#include "../../Vector2D.h"
#include "../../Singletons/GameTimeHelper.h"

class BasicWeapon : public Weapon
{
public:
	BasicWeapon();
	virtual ~BasicWeapon();

	virtual bool shoot(const Vector2D& bulletPosition, const Vector2D& direction);

};

#endif /* WEAPONS_PLAYERWEAPONS_BASICWEAPON_H_ */
