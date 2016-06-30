/*
 * SecondaryShipPU.h
 *
 *  Created on: Jun 22, 2016
 *      Author: gonzalo
 */

#ifndef POWERUPS_SECONDARYSHIPSPU_H_
#define POWERUPS_SECONDARYSHIPSPU_H_

#include "PowerUp.h"
class Player;

class SecondaryShipsPU: public PowerUp
{
public:
	SecondaryShipsPU();
    virtual ~SecondaryShipsPU();

    virtual void update();

    virtual void pickUp(Player* player);

};



#endif /* POWERUPS_SECONDARYSHIPSPU_H_ */
