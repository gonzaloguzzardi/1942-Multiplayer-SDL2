/*
 * ExtraPoints.h
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#ifndef POWERUPS_EXTRAPOINTSPU_H_
#define POWERUPS_EXTRAPOINTSPU_H_

#include "PowerUp.h"
class Player;

class ExtraPointsPU: public PowerUp
{
public:
	ExtraPointsPU();
	ExtraPointsPU(int points);
    virtual ~ExtraPointsPU();

    virtual void update();

    virtual void pickUp(Player* player);

private:

    int m_pointsToGive;

};



#endif /* POWERUPS_EXTRAPOINTSPU_H_ */
