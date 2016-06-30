/*
 * SecondaryShip.h
 *
 *  Created on: Jun 22, 2016
 *      Author: gonzalo
 */

#ifndef SECONDARYSHIP_H_
#define SECONDARYSHIP_H_

#include "MoveableObject.h"

class SecondaryShip: public MoveableObject
{
public:
	SecondaryShip();
	virtual ~SecondaryShip();

	virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

	virtual void update();

	void damage(int damage);
	void shoot(const Vector2D& bulletPosition, const Vector2D& direction, int speed, int ownerID, int teamID);

	bool isDead() {return m_dead;}
	void setDead(bool dead) {m_dead = dead;}


private:
    int m_health;

    bool m_dead;
    bool m_canRecycle;

    void sendDrawMessage(bool isAlive);
};



#endif /* SECONDARYSHIP_H_ */
