#ifndef ENEMIES_BOSSMATI_H_
#define ENEMIES_BOSSMATI_H_

#include "Enemy.h"
#include <math.h>
#include "../Movements/MovCircular.h"

#define SHOOT_CHANCE 60 // 0.05% de disparar por frame. Aproximadamente 10% de probabilidad de disparo por seg a 100fps como ahora

class BossMati: public Enemy
{
public:
	BossMati();
    virtual ~BossMati();

    virtual void update();

    virtual void shoot();

    virtual void damage(int damageReceived);

private:
    bool m_fleeing;
    int m_returnYOffset;
    int lastX;
    int lastY;
    int delay;
    bool m_flipping;
    int m_flipAnimationTime;
    int m_flipRemainingTime;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    void retire();
    void updateFlipAnimation();
    void updateExplotionAnimation();
    MovCircular* m_patronCirc;
};


#endif /* ENEMIES_BOSSMATI_H_ */
