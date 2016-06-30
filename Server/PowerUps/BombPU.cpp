/*
 * BombPU.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: gonzalo
 */

#include "BombPU.h"

#include "../Player.h"
#include "../Game.h"

BombPU::BombPU(): PowerUp()
{
}

BombPU::~BombPU()
{
}

void BombPU::update()
{
	PowerUp::update();
}

void BombPU::pickUp(Player* player)
{
	Game::Instance()->killAllEnemies(player);

	m_pickedUp = true;
	m_dead = true;
}
