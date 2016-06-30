/*
 * Nave.h
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#ifndef MOVEABLEOBJECT_H_
#define MOVEABLEOBJECT_H_

#include "GameObject.h"
//#include "Game.h"
//#include "Singletons/TextureManager.h"
#include "Vector2D.h"
#include <string>

#define DIRECTION_UP -1
#define DIRECTION_DOWN 1
#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT -1

class MoveableObject : public GameObject
{
public:
    virtual ~MoveableObject() {}

    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);
    
    //Funciones Generales
    virtual void draw();
    virtual void update();
    virtual void clean();
    virtual void collision();
    
    Vector2D& getDirection() { return m_direction; }
    Vector2D& getSpeed() { return m_speed; }



protected:

    MoveableObject();

    // variables de movimiento
    Vector2D m_direction;
    Vector2D m_speed;

};

#endif /* MOVEABLEOBJECT_H_ */
