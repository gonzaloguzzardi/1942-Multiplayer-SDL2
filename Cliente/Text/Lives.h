#ifndef LIVES_H_
#define LIVES_H_

#include "ScreenText.h"
#include "../Utils/TiposDefinidos.h"
#include "../Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <sstream>
#include <string>
#include <iomanip>

#define TEXT_SIZE_FACTOR 1.5f

class Lives: public ScreenText
{
public:

	Lives(int gameWidth,int gameHeight, int cantLives);
    ~Lives();
    void actualizarLives(int lives);
    void update();
    void reset();

private:

    //Textures
    FontTexture m_lives;

    int m_playerLives;
    int m_newLives;
    int m_initialLives;

    void updateTexture(short value);

};


#endif /* LIVES_H_ */
