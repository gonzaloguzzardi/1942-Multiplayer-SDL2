/*
 * DualWeaponPU.h
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#ifndef POWERUPS_DUALWEAPONPU_H_
#define POWERUPS_DUALWEAPONPU_H_

#include "PowerUp.h"
class Player;

class DualWeaponPU: public PowerUp
{
public:
	DualWeaponPU();
    virtual ~DualWeaponPU();

    virtual void update();

    virtual void pickUp(Player* player);

};



#endif /* POWERUPS_DUALWEAPONPU_H_ */
