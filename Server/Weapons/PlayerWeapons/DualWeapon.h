/*
 * DualWeapon.h
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_PLAYERWEAPONS_DUALWEAPON_H_
#define WEAPONS_PLAYERWEAPONS_DUALWEAPON_H_

#include "../Weapon.h"
#include "../../Vector2D.h"
#include "../../Singletons/GameTimeHelper.h"

class DualWeapon : public Weapon
{
public:
	DualWeapon();
	virtual ~DualWeapon();

	virtual bool shoot(const Vector2D& bulletPosition, const Vector2D& direction);

};



#endif /* WEAPONS_PLAYERWEAPONS_DUALWEAPON_H_ */
