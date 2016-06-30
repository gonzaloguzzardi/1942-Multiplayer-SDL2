/*
 * BombPU.h
 *
 *  Created on: Jun 13, 2016
 *      Author: gonzalo
 */

#ifndef POWERUPS_BOMBPU_H_
#define POWERUPS_BOMBPU_H_

#include "PowerUp.h"
class Player;

class BombPU: public PowerUp
{
public:
	BombPU();
    virtual ~BombPU();

    virtual void update();

    virtual void pickUp(Player* player);

};




#endif /* POWERUPS_BOMBPU_H_ */
