/*
 * BulletsHandler.h
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#ifndef WEAPONS_BULLETSHANDLER_H_
#define WEAPONS_BULLETSHANDLER_H_

#include <vector>
#include <pthread.h>
#include <memory>
class Bullet;

class BulletsHandler
{
public:

    static BulletsHandler* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new BulletsHandler();
            return s_pInstance;
        }
        return s_pInstance;
    }

    void addBullet(std::shared_ptr<Bullet> bullet);

    void updateBullets();

    void clearBullets();

    const std::vector<std::shared_ptr<Bullet>> getPlayerBullets() { return m_bullets; }

private:

    BulletsHandler();
    ~BulletsHandler();

    BulletsHandler(const BulletsHandler&);
    BulletsHandler& operator=(const BulletsHandler&);

    static BulletsHandler* s_pInstance;

    // in play bullets
	std::vector<std::shared_ptr<Bullet>> m_bullets;

	std::vector<std::shared_ptr<Bullet>> m_newBulletsToAdd;
	void addNewBullets();

	pthread_mutex_t  m_bulletsMutex;
};



#endif /* WEAPONS_BULLETSHANDLER_H_ */
