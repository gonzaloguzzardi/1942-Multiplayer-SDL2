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

#define SHOW_FPS 0

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char **argv)
{
    int frameStartTime, frameEndTime;

	std::cout << "Abriendo Server...\n";

	Random::initialize();

	if (Game::Instance()->init("1942 Servidor Ultra Diesel", 400, 150, 800, 600)) //flag por ejemplo: SDL_WINDOW_FULLSCREEN_DESKTOP

	{
		std::cout << "game init success!\n";

		//Bucle del juego
		while (Game::Instance()->isRunning())
		{
			frameStartTime = SDL_GetTicks();
			//Game::Instance()->handleEvents();
			Game::Instance()->update();

			if (USE_DRAWMESSAGE_PACKAGING)
			{
				Game::Instance()->sendPackages();
			}

			frameEndTime = SDL_GetTicks() - frameStartTime;


			//tiempo a esperar = tiempo que demoro en finalizar el  frame = tiempo en que finalizó - tiempo en que inició
			if (frameEndTime < DELAY_TIME) {
				SDL_Delay((int) ((DELAY_TIME - frameEndTime)));
				GameTimeHelper::Instance()->updateDeltaTime(DELAY_TIME);
				if (SHOW_FPS)
					printf("FPS: %d \n", (1000/ DELAY_TIME));
            }
			else
			{
				GameTimeHelper::Instance()->updateDeltaTime(frameEndTime);
				if (SHOW_FPS)
					printf("FPS: %d \n", (1000/ frameEndTime));
			}

        }

    }
    else
    {
        std::cout << "game init failure - " << SDL_GetError() << "\n";
        return -1;
    }

    std::cout << "game closing...\n";
    Game::Instance()->clean();

    return 0;
}
