#ifndef STAGEMSG_H_
#define STAGEMSG_H_

#include "../Utils/TiposDefinidos.h"
#include "../Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <list>

 #define TEXT_SIZE_FACTOR 1.5f

 class StageMsg: public ScreenText
 {
 public:
     StageMsg(int gameWidth,int gameHeight,StageBeginning info);
     ~StageMsg();

 private:

     void createFontTexture(short value, const char* text, FontTexture* font);

 };


 #endif /* STAGEMSG_H_ */
