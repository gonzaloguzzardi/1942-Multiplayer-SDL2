#ifndef RESULTS_H_
#define RESULTS_H_

#include "../Utils/TiposDefinidos.h"
#include "../Singletons/FontManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <list>

 #define TEXT_SIZE_FACTOR 1.5f

 class Results: public ScreenText
 {
 public:
     Results(int gameWidth,int gameHeight,FinishGameInfo info);
     ~Results();

 private:

     void generateTeamWin(int gameWidth, int gameHeight, short winnerID, short points);
     void generateGeneralWin(int gameWidth, int gameHeight, short winnerID, short points);
     void generateLoss(int gameWidth, int gameHeight);
     void generateDraw(int gameWidth, int gameHeight);
     void createFontTexture(short value, const char* text, FontTexture* font);

 };


 #endif /* RESULTS_H_ */
