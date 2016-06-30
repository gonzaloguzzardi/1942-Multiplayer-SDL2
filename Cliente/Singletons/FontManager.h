#ifndef SINGLETONS_FONTMANAGER_H_
#define SINGLETONS_FONTMANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../Utils/TiposDefinidos.h"
#include "../Game.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>


#define DEFAULT_FONT_PATH "Assets/Fonts/joystix2.ttf"
#define DEFAULT_FONT_SIZE 10

enum textquality {solid, shaded, blended};

class FontManager
{
public:

    static FontManager* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new FontManager();
            s_pInstance->init();
            return s_pInstance;
        }
        return s_pInstance;
    }

    bool loadfont(char* file, int ptsize);
    int textSize(const char* text,int *h,int* w);
    SDL_Texture* drawtext(char fgR, char fgG, char fgB, char fgA,
    	char bgR, char bgG, char bgB, char bgA, const char text[], textquality quality);

private:

    static FontManager* s_pInstance;
    TTF_Font* m_font;

    void init();
    bool validPath(const std::string& filePath);

    FontManager() {}
    ~FontManager() {}
    FontManager(const FontManager&);
	FontManager& operator=(const FontManager&);
};

#endif /* SINGLETONS_FONTMANAGER_H_ */
