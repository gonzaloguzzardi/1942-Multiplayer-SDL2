/*
 * EnemySpawner.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#include "EnemySpawner.h"
#include "../Enemies/Enemy.h"
#include "../Enemies/SmallEnemy.h"
#include "../Enemies/MediumPlane.h"
#include "../Enemies/Formation.h"
#include "../Enemies/BigPlane.h"
#include "../Utils/TextureHelper.h"
#include "../Utils/Random.h"
#include "../Game.h"
#include "../Singletons/CollisionHandler.h"

EnemySpawner::EnemySpawner()
{
	m_textureHelper = new TextureHelper();
}

EnemySpawner::~EnemySpawner()
{
	m_enemiesToSpawn.clear();
}

void EnemySpawner::clean()
{
	for (std::vector<EnemySpawnInfo>::iterator it = m_enemiesToSpawn.begin() ; it != m_enemiesToSpawn.end(); ++it)
	{
		if ((*it).enemyToSpawn)
		{
			delete (*it).enemyToSpawn;
		}
	}
	m_enemiesToSpawn.clear();
	if (m_textureHelper)
		delete m_textureHelper;
}

void EnemySpawner::reset()
{
	for (std::vector<EnemySpawnInfo>::iterator it = m_enemiesToSpawn.begin() ; it != m_enemiesToSpawn.end(); ++it)
	{
		if ((*it).enemyToSpawn)
		{
			delete (*it).enemyToSpawn;
		}
	}
	m_enemiesToSpawn.clear();
}


void EnemySpawner::feed(std::vector<Enemigo> enemies, int stageSize)
{
	m_stageSize = stageSize;

	for (std::vector<Enemigo>::iterator it = enemies.begin() ; it != enemies.end(); ++it)
	{
		addEnemy((*it));
	}
}

void EnemySpawner::update(int stagePosition)
{
	if (!Game::Instance()->isLevelStarted() || (Game::Instance()->isFinishingLevel()))
		return;

	for (std::vector<EnemySpawnInfo>::iterator it = m_enemiesToSpawn.begin() ; it != m_enemiesToSpawn.end();)
	{
		  if((*it).stagePosition <= (m_stageSize - stagePosition))
		  {
			  Game::Instance()->addEnemy((*it).enemyToSpawn);
			  (*it).enemyToSpawn->activateCollition();
			  //CollitionHandler::Instance()->addEnemy((*it).enemyToSpawn);
		      it = m_enemiesToSpawn.erase(it);
		  }
		  else
		  {
		     ++it;
		  }
	}
}

void EnemySpawner::addEnemy(Enemigo enemyInfo)
{
	if (enemyInfo.id.compare("AvionPequeño") == 0)
	{
		if (enemyInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(enemyInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / enemyInfo.cantidad;
		for(int i = 0; i < enemyInfo.cantidad; ++i)
		{
			EnemySpawnInfo enemySpawnInfo;
			Enemy* enemy = new SmallEnemy();
			int posX = Random::getRange(-1, 1);
			if (posX <= 0)
			{
				posX = 0 - enemyInfo.ancho;
			}
			else
			{
				posX = Game::Instance()->getGameWidth();
			}
			int posY = Random::getRange(0, Game::Instance()->getGameHeight()/1.5f);
			enemy->load(posX, posY, enemyInfo.ancho, enemyInfo.alto, textureID, enemyInfo.frames);
			enemySpawnInfo.enemyToSpawn = enemy;
			int randomStagePosition = Random::getRange((i * step) - (step/40), ((i + 1) * step) + (step/40));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			enemySpawnInfo.stagePosition = randomStagePosition;
			enemySpawnInfo.posX = posX;
			enemySpawnInfo.posY = posY;

		//	printf("Enemigo Pequeño en posicion %d \n", randomStagePosition);
			m_enemiesToSpawn.push_back(enemySpawnInfo);
		}
	}

	if (enemyInfo.id.compare("MediumPlane") == 0)
	{
		if (enemyInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(enemyInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / enemyInfo.cantidad;
		for(int i = 0; i < enemyInfo.cantidad; ++i)
		{
			EnemySpawnInfo enemySpawnInfo;
			Enemy* enemy = new MediumPlane();
			/*int posX = Random::getRange(-1, 1);
			if (posX <= 0)
			{
				posX = 0 - enemyInfo.ancho;
			}
			else
			{
				posX = Game::Instance()->getGameWidth();
			}
			int posY = Random::getRange(0, Game::Instance()->getGameHeight()/1.5f);
			*/
			int posX = Random::getRange(0, Game::Instance()->getGameWidth() - enemyInfo.ancho);
			int posY = 0;

			enemy->load(posX, posY, enemyInfo.ancho, enemyInfo.alto, textureID, enemyInfo.frames);
			enemySpawnInfo.enemyToSpawn = enemy;
			int randomStagePosition = Random::getRange((i * step) - (step/40), ((i + 1) * step) + (step/40));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			enemySpawnInfo.stagePosition = randomStagePosition;
			enemySpawnInfo.posX = posX;
			enemySpawnInfo.posY = posY;
			printf("Enemigo Mediano en posicion %d \n", randomStagePosition);

			m_enemiesToSpawn.push_back(enemySpawnInfo);
		}
	}

	if (enemyInfo.id.compare("FormacionRoja") == 0)
	{
		if (enemyInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(enemyInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / enemyInfo.cantidad;
		for(int i = 0; i < enemyInfo.cantidad; ++i)
		{
			EnemySpawnInfo enemySpawnInfo;
			Enemy* enemy;
			int posX = Random::getRange(-1, 1);
			if (posX <= 0)
			{
				posX = 0 - enemyInfo.ancho;
				enemy = new Formation(true);
			}
			else
			{
				posX = Game::Instance()->getGameWidth();
				enemy = new Formation(false);
			}
			int posY = Random::getRange(0, Game::Instance()->getGameHeight()/2);
			enemy->load(posX, posY, enemyInfo.ancho, enemyInfo.alto, textureID, enemyInfo.frames);
			enemySpawnInfo.enemyToSpawn = enemy;
			int randomStagePosition = Random::getRange((i * step) - (step/40), ((i + 1) * step) + (step/40));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			enemySpawnInfo.stagePosition = randomStagePosition;
			enemySpawnInfo.posX = posX;
			enemySpawnInfo.posY = posY;

			printf("Formación en posicion %d \n", randomStagePosition);

			m_enemiesToSpawn.push_back(enemySpawnInfo);
		}
	}
	if (enemyInfo.id.compare("AvionGrande") == 0)
	{
		if (enemyInfo.cantidad <= 0)
			return;

		int textureID = m_textureHelper->stringToInt(enemyInfo.id);
		int step = (m_stageSize - Game::Instance()->getGameHeight()) / enemyInfo.cantidad;
		for(int i = 0; i < enemyInfo.cantidad; ++i)
		{
			EnemySpawnInfo enemySpawnInfo;
			Enemy* enemy = new BigPlane();
			int posX = Random::getRange(0, Game::Instance()->getGameWidth() - enemyInfo.ancho);
			int posY = Game::Instance()->getGameHeight();
			enemy->load(posX, posY, enemyInfo.ancho, enemyInfo.alto, textureID, enemyInfo.frames);
			enemySpawnInfo.enemyToSpawn = enemy;
			int randomStagePosition = Random::getRange((i * step) - (step/20), ((i + 1) * step) + (step/20));
			if (randomStagePosition < 0)
				randomStagePosition = 0;
			if (randomStagePosition > m_stageSize)
				randomStagePosition = m_stageSize;
			enemySpawnInfo.stagePosition = randomStagePosition;
			enemySpawnInfo.posX = posX;
			enemySpawnInfo.posY = posY;
			printf("Enemigo Grande en posicion %d \n", randomStagePosition);
			m_enemiesToSpawn.push_back(enemySpawnInfo);
		}
	}

}
