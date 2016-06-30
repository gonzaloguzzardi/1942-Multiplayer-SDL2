/*
 * Background.h
 *
 *  Created on: Apr 22, 2016
 *      Author: gonzalo
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "BackgroundObject.h"
#include "../Vector2D.h"
#include "../Singletons/TextureManager.h"
#include "../Utils/TiposDefinidos.h"
#include "../Game.h"
#include <string>

class Background: public GameObject
{

public:
	Background();
	Background(const Vector2D& direction, const Vector2D& scrollSpeed);
	virtual ~Background();

	virtual void load(int x, int y, int width, int height, int textureID);
    virtual void draw();
    virtual void update();
    virtual void clean();

    //Getters
    Vector2D& getDirection() { return m_direction; }
    Vector2D& getScrollSpeed() { return m_scrollSpeed; }
    //Setters
    void setDirection(const Vector2D& direction) { m_direction = direction; }
    void setScrollSpeed(const Vector2D& scrollSpeed) { m_scrollSpeed = scrollSpeed; }

protected:

    //Variables de scroll
    Vector2D m_direction;
    Vector2D m_scrollSpeed;

};



#endif /* BACKGROUND_H_ */
