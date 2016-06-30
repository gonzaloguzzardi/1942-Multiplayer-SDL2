/*
 * Island.h
 *
 *  Created on: Apr 23, 2016
 *      Author: gonzalo
 */

#ifndef BACKGROUND_ISLAND_H_
#define BACKGROUND_ISLAND_H_

#include "../Game.h"
#include "../Singletons/TextureManager.h"
#include "RecurrentObject.h"


class Island: public RecurrentObject
{

public:
	Island();
	~Island();

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);
    virtual void draw();
    virtual void update();
    virtual void clean();

    virtual void reappear();

    void sendDrawMessage(bool isAlive);
};



#endif /* BACKGROUND_ISLAND_H_ */
