/*
 * GameTime.cpp
 *
 *  Created on: May 6, 2016
 *      Author: gonzalo
 */

#include "GameTimeHelper.h"

GameTimeHelper* GameTimeHelper::s_pInstance = 0;

void GameTimeHelper::updateDeltaTime(int deltaTime)
{
	m_deltaTime = deltaTime;
}

int GameTimeHelper::deltaTime()
{
	return m_deltaTime;
}
