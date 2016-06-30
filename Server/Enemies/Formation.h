/*
 * Formation.h
 *
 *  Created on: Jun 11, 2016
 *      Author: gonzalo
 */

#ifndef ENEMIES_FORMATION_H_
#define ENEMIES_FORMATION_H_

#include "Enemy.h"
#include <vector>
#include <map>
class FormationPlane;

class Formation: public Enemy
{
public:
	Formation(bool goingRight);
    virtual ~Formation();

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

    virtual void update();

    virtual void shoot();

    virtual bool damage(int damageReceived, bool wasShoot,  Player* damager);
    virtual void kill();

    void updateKillerStats(int playerID);

    virtual void activateCollition();

    void setGoingRight( bool goingRight ) { m_goingRight = goingRight; }

private:

    const int m_kFormationSize;
    int m_FormationRadius;

    std::vector<FormationPlane*> m_planes;
    std::map<int, int> m_playerIDKillsMap;

    int m_planesKilled;
    int m_pointsOnCombo;

    int m_lifeTime;
    int m_circleTime;
    int m_aliveTime;
    int m_disruptionTimer;
    int m_joiningScreenTimer;
    int m_canFleeTimer;
    bool m_canFlee;

    bool m_doingCircle;
    bool m_disruptingCircle;
    bool m_goingRight;
    bool m_fleeing;
    bool m_joiningScreen;

    Vector2D m_circleUpdatePosition;
    Vector2D m_disruptionDirection;

    void retire();
    void calculateRewards();
    void dropLoot(int probability);
    void updateAliveTime();
    void checkIfDead();

    void doCircleFormation();
    void updateCircleFormation();
    void updateDisruptingCircle();
    void checkDisruption();
    void keepInScreen();
    void updateCanFleeTimer();

    void updateParentPosition();

};




#endif /* ENEMIES_FORMATION_H_ */
