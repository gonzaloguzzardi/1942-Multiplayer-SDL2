/*
 * Bullet.h
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_BULLET_H_
#define WEAPONS_BULLET_H_

#include "../MoveableObject.h"
#include "../Singletons/GameTimeHelper.h"
#include <iostream>
class Game;

#define EXPLOTE 0

class Bullet : public MoveableObject
{
public:
	Bullet();
    virtual ~Bullet();

    virtual void update();
    virtual void clean();

    bool isDead();

    void sendDrawMessage(bool isAlive);

    virtual void kill();
    void setOwnerID(int ID) { m_ownerID = ID; }
    int getOwnerID() { return m_ownerID; }
    int getOwnerTeamNumber() { return m_ownerTeamNumber; }

    bool isExploting(){ return m_exploting; }
    void setDamage(int damage) { m_damage = damage; }
    int getDamage() { return m_damage; }
    void setOwnerTeam(int teamNumber) { m_ownerTeamNumber = teamNumber; }

protected:
    bool m_dead;
    bool m_exploting;
    //tiempo de vida en ms para autodestruirse
    int m_dyingTime;

    int m_explotionAnimationTime;
    int m_explotionRemainingTime;

    int m_damage;
    int m_ownerID;
    int m_ownerTeamNumber;

    void explote();
    void updateExplotionAnimation();

};




#endif /* WEAPONS_BULLET_H_ */
