/*
 * BigPlane.h
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_MEDIUMPLANE_H_
#define ENEMIES_MEDIUMPLANE_H_

#include "Enemy.h"
#include <map>

class MediumPlane: public Enemy
{
public:
	MediumPlane();
    virtual ~MediumPlane();

    virtual void update();
    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void shoot();

    virtual bool damage(int damageReceived, bool wasShoot,  Player* damager);
    virtual void kill();

private:

    std::map<int, int> m_playerIDDamageDone;

    const int m_shootChance;
    const int m_kMaxHealth;

    bool m_goingUp;
    bool m_goingRight;
    int m_pointsOnCombo;
    int frameC;

    int m_borderReturnOffset;
    Vector2D m_shootingOffset;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    void flip();
    void updateExplotionAnimation();
    void explote();
    void dropLoot();
    void updateKillerStats(int playerID, int damageDone);
    void calculateRewards();
    void updateAngle();

    void generateBorderReturnOffset();
};



#endif /* ENEMIES_MEDIUMPLANE_H_ */
