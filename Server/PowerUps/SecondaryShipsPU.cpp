/*
 * SecondaryShips.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: gonzalo
 */

#include "SecondaryShipsPU.h"

#include "../Player.h"
#include "../Game.h"

SecondaryShipsPU::SecondaryShipsPU(): PowerUp()
{
	m_textureID = 74;
	m_tag = "Power Up: Nave Secundaria";
}

SecondaryShipsPU::~SecondaryShipsPU()
{
}

void SecondaryShipsPU::update()
{
	PowerUp::update();
}

void SecondaryShipsPU::pickUp(Player* player)
{
	player->createSecondaryShips();

	m_pickedUp = true;
	m_dead = true;
}

