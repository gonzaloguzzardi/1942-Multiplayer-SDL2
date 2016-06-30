/*
 * BackgroundObject.h
 *
 *  Created on: Apr 22, 2016
 *      Author: gonzalo
 */

#ifndef BACKGROUNDOBJECT_H_
#define BACKGROUNDOBJECT_H_

#include "../GameObject.h"
//#include "../Game.h"
//#include "../Singletons/TextureManager.h"
#include "../Utils/Random.h"
#include "../Utils/TiposDefinidos.h"
#include <SDL2/SDL.h>
#include <iostream>


class RecurrentObject: public GameObject
{
public:
    virtual ~RecurrentObject() {}

    //Funciones Generales
    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);
    virtual void draw();
    virtual void update();
    virtual void clean();

    virtual void reappear();
    //Getters
    Vector2D& getDirection() { return m_direction; }
    Vector2D& getScrollSpeed() { return m_scrollSpeed; }
    Uint32 getReappearanceTime() { return m_reappearanceTime; }

    //Setters
    void setDirection(const Vector2D& direction) { m_direction = direction; }
    void setScrollSpeed(const Vector2D& scrollSpeed) { m_scrollSpeed = scrollSpeed; }
    void setReappearanceTime(Uint32 reappearanceTime) { m_reappearanceTime = reappearanceTime; }


protected:

    RecurrentObject();
    RecurrentObject(const Vector2D& direction, const Vector2D& scrollSpeed);

    // variables de movimiento
    Vector2D m_direction;
    Vector2D m_scrollSpeed;

    //tiempo para que vuelva a aparecer el objeto en pantalla en ms
    Uint32 m_reappearanceTime;
    bool m_waiting;

    //inicializado en 0. Lo necesita para remover el evento del callback en caso de que no se haya llamado y el timer se haya agregado
    SDL_TimerID timerID;
};





#endif /* BACKGROUNDOBJECT_H_ */
