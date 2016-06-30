/*
 * CircleBullet.h
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_ENEMYWEAPONS_CIRCLEBULLET_H_
#define WEAPONS_ENEMYWEAPONS_CIRCLEBULLET_H_

#include "../Bullet.h"

class CircleBullet : public Bullet
{
public:
	CircleBullet();
    virtual ~CircleBullet();

    virtual void update();
};



#endif /* WEAPONS_ENEMYWEAPONS_CIRCLEBULLET_H_ */
