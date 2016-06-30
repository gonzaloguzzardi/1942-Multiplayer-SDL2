/*
 * main.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: gonzalo
 */

#include "Game.h"
#include "Utils/Random.h"
#include <SDL2/SDL.h>
#include "Singletons/GameTimeHelper.h"
#include <iostream>

using namespace std;

const int FPS = 100;
const int DELAY_TIME = 1000.0f / FPS;


bool mainLoop() {
    Game::Instance()->createGame(DELAY_TIME);
    
    if (Game::Instance()->getRestart()) {
        std::cout << "Apretaste Restart\n";
        Game::Instance()->mrMusculo();
        Game::Instance()->setRunning(true);
        return true;
    }
    
    return false;
}

int main(int argc, char **argv)
{
    Random::initialize();
    
    if (Game::Instance()->initializeClient() == false) {
        return 0;
    }
    
    // Play the Game.
    bool continueLooping = true;
    while (continueLooping == true)
    {
        continueLooping = mainLoop();
        
        //printf("FPS PROMEDIO: %d \n", (fpsCount/ framesCount));
        std::cout << "game closing...\n";
        Game::Instance()->clean();
    }
}
