/*
 * FormationPlane.h
 *
 *  Created on: Jun 11, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_FORMATIONPLANE_H_
#define ENEMIES_FORMATIONPLANE_H_

#include "Enemy.h"
class Formation;

class FormationPlane: public Enemy
{
public:
	FormationPlane();
	FormationPlane(Formation* parent);
    virtual ~FormationPlane();

    virtual void update();

    virtual void shoot();

    virtual bool damage(int damageReceived, bool wasShoot,  Player* damager);
    virtual void kill();

    void updateAngle();

    void setDoingCircleAnimation(bool doingCircleAnimation) { m_doingCircleAnimation = doingCircleAnimation; }
    bool doingCircleAnimation() { return m_doingCircleAnimation; }
    void setDisruptingCircle(bool disruptingCircle) { m_disruptingCircle = disruptingCircle; }
    bool disruptingCircle() { return m_disruptingCircle; }

    void setFleeing(bool fleeing) { m_fleeing = fleeing; }

private:

    const int m_shootChance;

    Formation* m_formationParent;

    bool m_doingCircleAnimation;
    bool m_disruptingCircle;
    bool m_fleeing;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    void retire();
    void updateExplotionAnimation();
    void explote();
};



#endif /* ENEMIES_FORMATIONPLANE_H_ */
