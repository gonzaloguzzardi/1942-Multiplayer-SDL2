#ifndef SCREENTEXT_H_
#define SCREENTEXT_H_

#include "../Utils/TiposDefinidos.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <list>

#define TEXT_SIZE_FACTOR 1.5f
#define LERP_WEIGHT 0.1f

class ScreenText
{
public:
    virtual ~ScreenText();
    void draw(SDL_Renderer* renderer);

protected:
    std::list<FontTexture*> m_texts;

};


#endif /* SCREENTEXT_H_ */
