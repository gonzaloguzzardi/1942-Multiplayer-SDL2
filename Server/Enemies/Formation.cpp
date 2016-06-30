/*
 * Formation.cpp
 *
 *  Created on: Jun 11, 2016
 *      Author: gonzalo
 */

#include "Formation.h"
#include "../Game.h"
#include "../Singletons/CollisionHandler.h"
#include "../Vector2D.h"
#include "../Weapons/EnemyWeapons/EnemyBaseGun.h"
#include "../Player.h"
#include "FormationPlane.h"
#include "../Singletons/GameTimeHelper.h"
#include "../PowerUps/DualWeaponPU.h"
#include "../PowerUps/PowerUp.h"
#include "../PopUps/PointsPopUp.h"


Formation::Formation(bool goingRight) :Enemy(),
						m_kFormationSize(5),
						m_planesKilled(0),
						m_pointsOnCombo(1000),
						m_aliveTime(0),
						m_disruptionTimer(0),
						m_canFleeTimer(500),
						m_canFlee(false),
						m_doingCircle(false),
						m_disruptingCircle(false),
						m_goingRight(goingRight),
						m_fleeing(false),
						m_joiningScreen(true)
{
	m_tag = "Formation";
	m_speed = Vector2D(2.5f, 2.5f);
	m_lifeTime = Random::getRange(8500, 12500);
	m_circleTime = Random::getRange(500, 2000);
	m_joiningScreenTimer = 1000;

	for (int i = 0; i < m_kFormationSize; ++i)
	{
		FormationPlane* plane = new FormationPlane(this);
		plane->load(m_position.m_x, m_position.m_y, 48, 48, 32, 1);
		plane->setSpeed(m_speed);
		m_planes.push_back(plane);
	}

	m_FormationRadius = m_kFormationSize * 24;

}


Formation::~Formation()
{
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i])
		{
			delete m_planes[i];
		}
	}
	m_planes.clear();
	m_playerIDKillsMap.clear();
}

void Formation::activateCollition()
{
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i] && (!(m_planes[i]->isDead())))
		{
			CollitionHandler::Instance()->addEnemy(m_planes[i]);
		}
	}
}

void Formation::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	Enemy::load(x, y, width, height, textureID, numFrames);
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i])
		{
			int posX = m_position.m_x - (i * 48) - 16;
			int posY = m_position.m_y;
			m_planes[i]->setPosition(Vector2D(posX, posY));
			if (m_goingRight)
			{
				m_planes[i]->setDirection(Vector2D(DIRECTION_RIGHT, 0));
			}
			else
			{
				m_planes[i]->setDirection(Vector2D(DIRECTION_LEFT, 0));
			}
			m_planes[i]->updateAngle();
		}
	}

}

void Formation::update()
{
	//actualiza timer de vida para retirarse
	updateAliveTime();
	updateCanFleeTimer();

	//Si es el tiempo de hacer la formacion circular, la hace
	if ((!m_doingCircle) && (m_aliveTime >= m_circleTime))
	{
		doCircleFormation();
	}
	if (m_doingCircle)
	{
		updateCircleFormation();
	}

	if (m_disruptingCircle)
	{
		updateDisruptingCircle();
	}

	if (!m_fleeing)
	{
		keepInScreen();
	}

	updateParentPosition();
	checkIfDead();
	//Actualiza la posición y comunica a los clientes
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i] && (!(m_planes[i]->isDead())))
		{
			m_planes[i]->update();
		}
	}
}

void Formation::updateCanFleeTimer()
{
	if (m_canFleeTimer > 0)
	{
		m_canFleeTimer -= GameTimeHelper::Instance()->deltaTime();
	}
	else
	{
		m_canFlee = true;
	}
}

void Formation::updateParentPosition()
{
	if (!m_doingCircle && m_goingRight)
	{
		m_direction = m_planes[0]->getDirection();
	}
	if (!m_doingCircle && !m_goingRight)
	{
		m_direction = m_planes[m_kFormationSize - 1]->getDirection();
	}

	if (m_doingCircle)
	{
		m_direction = Vector2D(0, 0);
	}
	m_position.m_x += (m_direction.m_x * m_speed.m_x);
	m_position.m_y += (m_direction.m_y * m_speed.m_y);
}

void Formation::doCircleFormation()
{
	m_doingCircle = true;
	if (m_goingRight)
	{
		m_circleUpdatePosition = m_planes[0]->getPosition();
		m_position.m_x += m_width/2;
		m_position.m_y += m_height/2;
	}
	else
	{
		m_circleUpdatePosition = m_planes[m_kFormationSize - 1]->getPosition();
		m_position.m_x -=  ((m_kFormationSize - 1) * 48) - 16 + m_width/2;
		m_position.m_y += m_height/2;
	}
	m_disruptionTimer = Random::getRange(2000, 5000);
}

void Formation::updateCircleFormation()
{
	m_disruptionTimer -= GameTimeHelper::Instance()->deltaTime();

	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i] && (!(m_planes[i]->isDead())) && (!(m_planes[i]->isDying())))
		{
			if ((!(m_planes[i]->doingCircleAnimation())) && (!(m_planes[i]->disruptingCircle())))
			{
				Vector2D difference = m_planes[i]->getPosition() - m_circleUpdatePosition;
				if (difference.length() <= 1.5f)
				{
					m_planes[i]->setDoingCircleAnimation(true);
				}
			}
			else
			{
				if (m_planes[i]->disruptingCircle())
					continue;

				Vector2D vectorToCenter = (m_position - m_planes[i]->getPosition());
				vectorToCenter.normalize();
				Vector2D perpendicular = Vector2D(-vectorToCenter.m_y, vectorToCenter.m_x) / vectorToCenter.length();

				m_planes[i]->setDirection(perpendicular);
				m_planes[i]->updateAngle();
			}
		}
	}
	checkDisruption();
}

void Formation::checkIfDead()
{
	int deadCount = 0;
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i]->isDead())
		{
			++deadCount;
			continue;
		}
	}
	if (deadCount >= m_kFormationSize)
	{
		m_dead = true;
	}
}

void Formation::checkDisruption()
{
	if (m_goingRight)
	{
		if (m_planes[m_kFormationSize - 1]->disruptingCircle())
		{
			m_doingCircle = false;
			return;
		}
	}
	else
	{
		if (m_planes[0]->disruptingCircle())
		{
			m_doingCircle = false;
			return;
		}
	}

	if (m_disruptionTimer <= 0)
	{
		m_disruptingCircle = true;
		if (m_goingRight)
		{
			if (m_planes[0]->disruptingCircle())
				return;
			m_planes[0]->setDisruptingCircle(true);
			m_planes[0]->setDoingCircleAnimation(false);
			m_disruptionDirection = Vector2D(m_planes[0]->getDirection().m_x, m_planes[0]->getDirection().m_y);
		}
		else
		{
			if (m_planes[m_kFormationSize - 1]->disruptingCircle())
				return;
			m_planes[m_kFormationSize - 1]->setDisruptingCircle(true);
			m_planes[m_kFormationSize - 1]->setDoingCircleAnimation(false);
			m_disruptionDirection = Vector2D(m_planes[m_kFormationSize - 1]->getDirection().m_x, m_planes[m_kFormationSize - 1]->getDirection().m_y);
		}
	}
}

void Formation::updateDisruptingCircle()
{
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i]->disruptingCircle())
			continue;

		//Actualiza el resto de los aviones para que dejen de dar vueltas
		Vector2D directionDifference = m_planes[i]->getDirection() - m_disruptionDirection;
		if ((m_planes[i]->disruptingCircle() == false) && (directionDifference.length() < 0.1f))
		{
			m_planes[i]->setDirection(Vector2D(m_disruptionDirection.m_x, m_disruptionDirection.m_y));
			m_planes[i]->setDisruptingCircle(true);
			m_planes[i]->setDoingCircleAnimation(false);
		}
	}
}

void Formation::keepInScreen()
{
	if (m_doingCircle || m_joiningScreen)
	{
		return;
	}

	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i]->doingCircleAnimation())
		{
			continue;
		}

		bool dirty = false;
		float directionX = m_planes[i]->getDirection().getX();
		float directionY = m_planes[i]->getDirection().getY();

		if ((m_planes[i]->getPosition().getX() - 48) < 0)
		{
			if (directionX < 0)
				directionX *=  (-1);
			dirty = true;
		}
		if (m_planes[i]->getPosition().getX() > Game::Instance()->getGameWidth())
		{
			if (!dirty)
			{
				if (directionX > 0)
					directionX *=  (-1);
				dirty = true;
			}
		}
		if ((m_planes[i]->getPosition().getY()  - 48) < 0)
		{
			if (!dirty)
			{
				if (directionY < 0)
					directionY *=  (-1);
				dirty = true;
			}
		}
		if (m_planes[i]->getPosition().getY() > Game::Instance()->getGameHeight())
		{
			if (!dirty)
			{
				if (directionY > 0)
					directionY *=  (-1);
				dirty = true;
			}
		}

		if (dirty)
		{
			m_canFlee = false;
			m_canFleeTimer = 1000;
			Vector2D newDireciton = Vector2D(directionX, directionY);
			//newDireciton.normalize();
			m_planes[i]->setDirection(newDireciton);
			m_planes[i]->updateAngle();
		}
	}
}

bool Formation::damage(int damageReceived, bool wasShoot,  Player* damager)
{
	int killCount = 0;
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		bool killed = m_planes[i]->damage(damageReceived, wasShoot, damager);
		if (killed)
			++killCount;
	}
	if (killCount >= m_kFormationSize)
		return true;

	return false;
}

void Formation::kill()
{
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		m_planes[i]->kill();
	}
	m_dead = true;
}


void Formation::updateAliveTime()
{
	m_aliveTime += GameTimeHelper::Instance()->deltaTime();
	if ((m_aliveTime >= m_lifeTime) && (!m_fleeing))
	{
		retire();
	}

	if (m_joiningScreen)
	{
		m_joiningScreenTimer -= GameTimeHelper::Instance()->deltaTime();
		if (m_joiningScreenTimer <= 0)
		{
			m_joiningScreen = false;
		}
	}
}

void Formation::updateKillerStats(int playerID)
{
	if (m_playerIDKillsMap.find(playerID) == m_playerIDKillsMap.end())
	{
		//nuevo killer
		m_playerIDKillsMap[playerID] = 1;
	}
	else
	{
		//ya habia matado a algun avion del a formacion
		m_playerIDKillsMap[playerID] = m_playerIDKillsMap[playerID] + 1;
	}

	++m_planesKilled;

	if (m_planesKilled >= m_kFormationSize)
	{
		//Todos los aviones han sido matados
		calculateRewards();
		m_dead = true;
	}
}

void Formation::calculateRewards()
{
	bool loot = false;
	for (std::map<int,int>::iterator it = m_playerIDKillsMap.begin(); it != m_playerIDKillsMap.end(); ++it)
	{
		if (it->second >= m_kFormationSize)
		{
			//Todos los aviones fueron derrotados por un mismo jugador
			int playerKillerID = it->first;
			//busca el equipo del player que derribo a todos los aviones de la formación
			int teamKillerID = Game::Instance()->getPlayerTeam(playerKillerID);

			//Suma los puntos
			if ((playerKillerID != -1) && (teamKillerID != -1))
			{
				Game::Instance()->addPointsToScore(m_pointsOnCombo, playerKillerID, teamKillerID);
				PopUp* pointsPopUp = new PointsPopUp(playerKillerID, m_pointsOnCombo);
				pointsPopUp->load(m_position.m_x + m_width/2, m_position.m_y + m_height/2, 96, 16, 104, 1);
				Game::Instance()->addPopUp(pointsPopUp);
			}
			//dropea loot con un 50% de chance
			dropLoot(50);
			loot = true;
		}
	}

	//Si la formacion fue matada por diferentes jugadores, dropea loot con 25% de probabilidades
	if (!loot)
	{
		dropLoot(25);
	}
}

void Formation::shoot()
{
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		if (m_planes[i] && (!(m_planes[i]->isDead())) && (!((m_planes[i]->isDying()))))
		{
			m_planes[i]->shoot();
		}
	}
}

void Formation::retire()
{
	if (!m_canFlee)
		return;

	m_fleeing = true;
	for (int i = 0; i < m_kFormationSize; ++i)
	{
		m_planes[i]->setFleeing(true);
	}
}

void Formation::dropLoot(int probability)
{
	if (probability < Random::getRange(0, 100))
	{
		PowerUp* drop = new DualWeaponPU();
		int posX = m_position.m_x + (m_width/2) - 24;
		int posY = m_position.m_y + (m_height/2) - 24;
		drop->load(posX, posY, 48, 48, 72, 1);
		CollitionHandler::Instance()->addPowerUp(drop);
		Game::Instance()->addPowerUp(drop);
	}
}

