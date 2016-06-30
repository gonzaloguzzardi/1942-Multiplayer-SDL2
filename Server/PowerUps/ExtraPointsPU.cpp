/*
 * ExtraPoints.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#include "ExtraPointsPU.h"

#include "../Player.h"
#include "../Game.h"
#include "../PopUps/PointsPopUp.h"

ExtraPointsPU::ExtraPointsPU(): PowerUp(),
							m_pointsToGive(100)
{
}

ExtraPointsPU::ExtraPointsPU(int points): PowerUp(),
							m_pointsToGive(points)
{
}

ExtraPointsPU::~ExtraPointsPU()
{
}

void ExtraPointsPU::update()
{
	PowerUp::update();
}

void ExtraPointsPU::pickUp(Player* player)
{
	Game::Instance()->addPointsToScore(m_pointsToGive, player->getObjectId(), player->getPlayerTeam().gameTeamID);
	PopUp* pointsPopUp = new PointsPopUp(player->getObjectId(), m_pointsToGive);
	pointsPopUp->load(m_position.m_x + m_width/2, m_position.m_y + m_height/2, 96, 16, 104, 1);
	Game::Instance()->addPopUp(pointsPopUp);

	m_pickedUp = true;
	m_dead = true;
}
