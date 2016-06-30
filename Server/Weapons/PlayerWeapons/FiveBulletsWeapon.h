/*
 * FiveBulletsWeapon.h
 *
 *  Created on: May 9, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_PLAYERWEAPONS_FIVEBULLETSWEAPON_H_
#define WEAPONS_PLAYERWEAPONS_FIVEBULLETSWEAPON_H_

#include "../Weapon.h"
#include "../../Vector2D.h"

class FiveBulletsWeapon : public Weapon
{
public:
	FiveBulletsWeapon();
	virtual ~FiveBulletsWeapon();

	virtual bool shoot(const Vector2D& bulletPosition, const Vector2D& direction);

};



#endif /* WEAPONS_PLAYERWEAPONS_FIVEBULLETSWEAPON_H_ */
