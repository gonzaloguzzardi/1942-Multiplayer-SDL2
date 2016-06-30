/*
 * DualWeaponPU.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: gonzalo
 */

#include "DualWeaponPU.h"

#include "../Player.h"
#include "../Game.h"
#include "../Weapons/PlayerWeapons/DualWeapon.h"

DualWeaponPU::DualWeaponPU(): PowerUp()
{
}

DualWeaponPU::~DualWeaponPU()
{
}

void DualWeaponPU::update()
{
	PowerUp::update();
}

void DualWeaponPU::pickUp(Player* player)
{
	DualWeapon* dualWeapon = new DualWeapon();
	Game::Instance()->changePlayerWeapon(dualWeapon, player);

	m_pickedUp = true;
	m_dead = true;
}
