/*
 * DobleBullet.h
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_DOUBLEBULLET_H_
#define WEAPONS_DOUBLEBULLET_H_

#include "Bullet.h"

class DoubleBullet : public Bullet
{
public:
	DoubleBullet();
    virtual ~DoubleBullet();

    virtual void update();
};




#endif /* WEAPONS_DOUBLEBULLET_H_ */
