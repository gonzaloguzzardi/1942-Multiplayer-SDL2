/*
 * PowerUpSpawner.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#include "PowerUpSpawner.h"
#include "../PowerUps/PowerUp.h"
#include "../PowerUps/ExtraPointsPU.h"
#include "../PowerUps/BombPU.h"
#include "../PowerUps/DualWeaponPU.h"
#include "../PowerUps/SecondaryShipsPU.h"
#include "../Utils/TextureHelper.h"
#include "../Utils/Random.h"
#include "../Game.h"
#include "../Singletons/CollisionHandler.h"

PowerUpSpawner::PowerUpSpawner()
{
	m_textureHelper = new TextureHelper();
}

PowerUpSpawner::~PowerUpSpawner()
{
	m_powerUpsToSpawn.clear();
}

void PowerUpSpawner::clean()
{
	for (std::vector<PowerUpSpawnInfo>::iterator it = m_powerUpsToSpawn.begin() ; it != m_powerUpsToSpawn.end(); ++it)
	{
		if ((*it).powerUpToSpawn)
		{
			delete (*it).powerUpToSpawn;
		}
	}
	m_powerUpsToSpawn.clear();

	if (m_textureHelper)
		delete m_textureHelper;
}

void PowerUpSpawner::feed(std::vector<Powerup> powerUps, int stageSize)
{
	m_stageSize = stageSize;

	for (std::vector<Powerup>::iterator it = powerUps.begin() ; it != powerUps.end(); ++it)
	{
		addPowerUp((*it));
	}
}

void PowerUpSpawner::update(int stagePosition)
{
	if (!Game::Instance()->isLevelStarted() || (Game::Instance()->isFinishingLevel()))
		return;

	for (std::vector<PowerUpSpawnInfo>::iterator it = m_powerUpsToSpawn.begin() ; it != m_powerUpsToSpawn.end();)
	{
		  if((*it).stagePosition <= (m_stageSize - stagePosition))
		  {
			  Game::Instance()->addPowerUp((*it).powerUpToSpawn);
			  CollitionHandler::Instance()->addPowerUp((*it).powerUpToSpawn);
		      it = m_powerUpsToSpawn.erase(it);
		  }
		  else
		  {
		     ++it;
		  }
	}
}

void PowerUpSpawner::reset()
{
	for (std::vector<PowerUpSpawnInfo>::iterator it = m_powerUpsToSpawn.begin() ; it != m_powerUpsToSpawn.end(); ++it)
	{
		if ((*it).powerUpToSpawn)
		{
			delete (*it).powerUpToSpawn;
		}
	}
	m_powerUpsToSpawn.clear();
}


void PowerUpSpawner::addPowerUp(Powerup powerUpInfo)
{
	if (powerUpInfo.id.compare("ExtraPoints") == 0)
	{
		if (powerUpInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(powerUpInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / powerUpInfo.cantidad;
		for(int i = 0; i < powerUpInfo.cantidad; ++i)
		{
			PowerUpSpawnInfo powerUpSpawnInfo;
			int pointsValue = getRandomPointsValue();
			PowerUp* powerUp = new ExtraPointsPU(pointsValue);
			int posX = Random::getRange(0, Game::Instance()->getGameWidth() - powerUpInfo.ancho);
			int posY = Random::getRange(0, (Game::Instance()->getGameHeight() - (Game::Instance()->getGameHeight()/5)));
			powerUp->load(posX, posY, powerUpInfo.ancho, powerUpInfo.alto, textureID, powerUpInfo.frames);
			powerUpSpawnInfo.powerUpToSpawn = powerUp;

			int randomStagePosition = Random::getRange((i * step) - (step/50), ((i + 1) * step) + (step/50));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			powerUpSpawnInfo.stagePosition = randomStagePosition;
			powerUpSpawnInfo.posX = posX;
			powerUpSpawnInfo.posY = posY;

			//printf("Extra Points Power Up en posicion %d \n", randomStagePosition);
			m_powerUpsToSpawn.push_back(powerUpSpawnInfo);
		}
	}

	if (powerUpInfo.id.compare("BombPU") == 0)
	{
		if (powerUpInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(powerUpInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / powerUpInfo.cantidad;
		for(int i = 0; i < powerUpInfo.cantidad; ++i)
		{
			PowerUpSpawnInfo powerUpSpawnInfo;
			PowerUp* powerUp = new BombPU();
			int posX = Random::getRange(0, Game::Instance()->getGameWidth() - powerUpInfo.ancho);
			int posY = Random::getRange(0, (Game::Instance()->getGameHeight() - (Game::Instance()->getGameHeight()/4)));
			powerUp->load(posX, posY, powerUpInfo.ancho, powerUpInfo.alto, textureID, powerUpInfo.frames);
			powerUpSpawnInfo.powerUpToSpawn = powerUp;

			int randomStagePosition = Random::getRange((i * step) - (step/50), ((i + 1)* step) + (step/50));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			powerUpSpawnInfo.stagePosition = randomStagePosition;
			powerUpSpawnInfo.posX = posX;
			powerUpSpawnInfo.posY = posY;

			//printf("Bomb Power Up en posicion %d \n", randomStagePosition);
			m_powerUpsToSpawn.push_back(powerUpSpawnInfo);
		}
	}
	if (powerUpInfo.id.compare("DualWeaponPU") == 0)
	{
		if (powerUpInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(powerUpInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / powerUpInfo.cantidad;
		for(int i = 0; i < powerUpInfo.cantidad; ++i)
		{
			PowerUpSpawnInfo powerUpSpawnInfo;
			PowerUp* powerUp = new DualWeaponPU();
			int posX = Random::getRange(0, Game::Instance()->getGameWidth() - powerUpInfo.ancho);
			int posY = Random::getRange(0, (Game::Instance()->getGameHeight() - (Game::Instance()->getGameHeight()/4)));
			powerUp->load(posX, posY, powerUpInfo.ancho, powerUpInfo.alto, textureID, powerUpInfo.frames);
			powerUpSpawnInfo.powerUpToSpawn = powerUp;

			int randomStagePosition = Random::getRange((i * step) - (step/50), ((i + 1) * step) + (step/50));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			powerUpSpawnInfo.stagePosition = randomStagePosition;
			powerUpSpawnInfo.posX = posX;
			powerUpSpawnInfo.posY = posY;

			//printf("Bomb Power Up en posicion %d \n", randomStagePosition);
			m_powerUpsToSpawn.push_back(powerUpSpawnInfo);
		}
	}
	if (powerUpInfo.id.compare("SecondaryPow") == 0)
	{
		if (powerUpInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(powerUpInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / powerUpInfo.cantidad;
		for(int i = 0; i < powerUpInfo.cantidad; ++i)
		{
			PowerUpSpawnInfo powerUpSpawnInfo;
			PowerUp* powerUp = new SecondaryShipsPU();
			int posX = Random::getRange(0, Game::Instance()->getGameWidth() - powerUpInfo.ancho);
			int posY = Random::getRange(0, (Game::Instance()->getGameHeight() - (Game::Instance()->getGameHeight()/4)));
			powerUp->load(posX, posY, powerUpInfo.ancho, powerUpInfo.alto, textureID, powerUpInfo.frames);
			powerUpSpawnInfo.powerUpToSpawn = powerUp;

			int randomStagePosition = Random::getRange((i * step) - (step/50), ((i + 1) * step) + (step/50));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			powerUpSpawnInfo.stagePosition = randomStagePosition;
			powerUpSpawnInfo.posX = posX;
			powerUpSpawnInfo.posY = posY;

			//printf("Bomb Power Up en posicion %d \n", randomStagePosition);
			m_powerUpsToSpawn.push_back(powerUpSpawnInfo);
		}
	}

}

int PowerUpSpawner::getRandomPointsValue()
{
	//7.5% drop 50 pts
	//10% drop 100 pts
	//17.5% drop 250 pts
	//25% drop 500 pts
	//17.5% drop 750 pts
	//10% drop 1000 pts
	//7% drop 1500 pts
	//3% drop 3000 pts
	//1.5% drop 5000 pts
	//1% drop 10000 pts

	int luck = Random::getRange(0, 1000);
	if (luck < 75)
	{
		return 50;
	}

	if ((luck >= 75) && (luck < 175))
	{
		return 100;
	}

	if ((luck >= 175) && (luck < 350))
	{
		return 250;
	}

	if ((luck >= 350) && (luck < 600))
	{
		return 500;
	}

	if ((luck >= 600) && (luck < 775))
	{
		return 750;
	}

	if ((luck >= 775) && (luck < 875))
	{
		return 1000;
	}

	if ((luck >= 875) && (luck < 945))
	{
		return 1500;
	}

	if ((luck >= 945) && (luck < 975))
	{
		return 3000;
	}

	if ((luck >= 975) && (luck < 990))
	{
		return 5000;
	}

	if ((luck >= 990) && (luck <= 1000))
	{
		return 10000;
	}

	//Default
	return 500;
}
