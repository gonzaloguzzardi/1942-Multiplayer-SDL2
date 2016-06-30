/*
 * Portaviones.h
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#ifndef BACKGROUND_PORTAVIONES_H_
#define BACKGROUND_PORTAVIONES_H_

#include "../Game.h"
#include "../Singletons/TextureManager.h"
#include "RecurrentObject.h"

class Portaviones: public RecurrentObject
{
public:
	Portaviones();
	~Portaviones();

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);
    virtual void draw();
    virtual void update();
    virtual void clean();

    virtual void reappear();

    void sendDrawMessage(bool isAlive);
};




#endif /* BACKGROUND_PORTAVIONES_H_ */
