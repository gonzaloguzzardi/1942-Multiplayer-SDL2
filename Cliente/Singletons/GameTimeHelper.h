/*
 * GameTime.h
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_GAMETIME_H_
#define UTILS_GAMETIME_H_

#include <SDL2/SDL.h>

class GameTimeHelper
{
public:
    static GameTimeHelper* Instance()
    {
        if(s_pInstance == 0)
        {
            s_pInstance = new GameTimeHelper();
            return s_pInstance;
        }
        return s_pInstance;
    }

    void updateDeltaTime(int frameStart);

    //delta time en milisegundos
    int deltaTime();

private:
    static GameTimeHelper* s_pInstance;
    int m_deltaTime;

    GameTimeHelper(): m_deltaTime(0) {}
    ~GameTimeHelper() {}
    GameTimeHelper(const GameTimeHelper&);
    GameTimeHelper& operator=(const GameTimeHelper&);
};



#endif /* UTILS_GAMETIME_H_ */
