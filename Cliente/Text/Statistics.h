#ifndef STATISTICS_H_
 #define STATISTICS_H_

#include "../Utils/TiposDefinidos.h"
#include "../Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <list>

 #define TEXT_SIZE_FACTOR 1.5f

 class Statistics: public ScreenText
 {
 public:
     Statistics(int gameWidth,int gameHeight,StageStatistics stats);
     ~Statistics();

 private:

     void createFontTexture(short value, const char* text, FontTexture* font);

 };


 #endif /* STATISTICS_H_ */
