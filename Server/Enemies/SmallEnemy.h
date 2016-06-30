/*
 * SmallEnemy.h
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_SMALLENEMY_H_
#define ENEMIES_SMALLENEMY_H_

#include "Enemy.h"
#include <math.h>
class Player;

class SmallEnemy: public Enemy
{
public:
    SmallEnemy();
    virtual ~SmallEnemy();

    virtual void update();

    virtual void shoot();

    virtual bool damage(int damageReceived, bool wasShoot,  Player* damager);
    virtual void kill();

private:

    const int m_shootChance;

    bool m_fleeing;
    int m_returnYOffset;

    bool m_flipping;
    int m_flipAnimationTime;
    int m_flipRemainingTime;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    void retire();
    void updateFlipAnimation();
    void updateExplotionAnimation();
    void explote();

};


#endif /* ENEMIES_SMALLENEMY_H_ */
