/*
 * Island.h
 *
 *  Created on: Apr 23, 2016
 *      Author: gonzalo
 */

#ifndef BACKGROUND_ISLAND_H_
#define BACKGROUND_ISLAND_H_

#include "BackgroundObject.h"
#include "../Game.h"
#include "../Singletons/TextureManager.h"


class Island: public BackgroundObject
{

public:
	Island();
	~Island();

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);
    virtual void draw();
    virtual void update();

    virtual void reappear();
};



#endif /* BACKGROUND_ISLAND_H_ */
