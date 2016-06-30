/*
 * InputHandler.h
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#ifndef SINGLETONS_INPUTHANDLER_H_
#define SINGLETONS_INPUTHANDLER_H_

#include "../Game.h"
#include "../Vector2D.h"

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>


enum MOUSE_BUTTONS
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

/*POR AHORA SOLO SIRVE PARA VER LA POSICION DONDE ESTA EL MOUSE,
 *  EL ESTADO DE LOS 3 CLICKS DE MOUSE
 * Y QUE TECLA ESTA SIENDO PRESIONADA DEL TECLADO.
 * CON ESTO SOLO SE PUEDE SIMULAR LOS EVENTOS ONKEY DOWN/UP y ONMOUSE DOWN/UP SI SE REQUIERE MAYOR COMPLEJIDAD.
 * Y SINO PODEMOS HACER UN EVENT HANDLER O ROMPER EMNCAPSULAMIENTO Y HACER HACER UN MINI INPUTHANDLER DENTRO DE LA CLASE QUE REQUIERA DICHA COMPLEJIDAD
*/

class InputHandler
{
public:

    static InputHandler* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new InputHandler();
        }

        return s_pInstance;
    }


    void reset();

    // update and clean the input handler
    void update();
    void clean();

    // Eventos de Teclaso
    //Devuelve si la tecla con el código especificado está siendo presionada
    bool isKeyDown(SDL_Scancode key) const;

    // Eventos de Mouse
    //Devuelve
    bool getMouseButtonState(int buttonNumber) const;
    Vector2D* getMousePosition() const;

private:

    // Inputs de teclado
    void onKeyDown();
    void onKeyUp();

    const Uint8* m_keystates;

    // Inputs de mouse
    void onMouseMove(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    std::vector<bool> m_mouseButtonStates;
    Vector2D* m_mousePosition;

    static InputHandler* s_pInstance;

    InputHandler();
    ~InputHandler();
    InputHandler(const InputHandler&);
	InputHandler& operator=(const InputHandler&);
};



#endif /* SINGLETONS_INPUTHANDLER_H_ */
