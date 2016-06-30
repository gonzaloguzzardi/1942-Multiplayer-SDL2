/*
 * BlueBuller.h
 *
 *  Created on: May 9, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_BLUEBULLET_H_
#define WEAPONS_BLUEBULLET_H_

#include "Bullet.h"


class BlueBullet : public Bullet
{
public:
	BlueBullet();
    virtual ~BlueBullet();

    virtual void update();
    bool action;
};



#endif /* WEAPONS_BLUEBULLET_H_ */
