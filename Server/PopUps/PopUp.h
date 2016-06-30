/*
 * PopUp.h
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#include "../MoveableObject.h"

class PopUp: public MoveableObject
{
public:
	PopUp();
	PopUp(int ownerID);
    virtual ~PopUp();
    virtual void update();
    void sendDrawMessage(bool isAlive);

    void setOwnerID(int ownerID) { m_ownerID = ownerID; }
    int getOwnerID() { return m_ownerID; }
    bool isDead(){ return m_dead; }
    void setDead(bool dead) { m_dead = dead; }

protected:

    const int m_kDissappearanceTime;

	int m_ownerID;
    int m_remainingTime;

    bool m_dead;

    virtual void animatePopUp();
};

