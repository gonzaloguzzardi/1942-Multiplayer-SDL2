/*
 * BigPlane.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#include "BigPlane.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "../Weapons/EnemyWeapons/EnemyBigBaseGun.h"
#include "../Player.h"
#include "../PowerUps/ExtraPointsPU.h"
#include "../PowerUps/PowerUp.h"
#include "../Singletons/CollisionHandler.h"
#include "../PopUps/PointsPopUp.h"


BigPlane::BigPlane() :Enemy(),
					  m_shootChance(10),
					  m_kMaxHealth(1000),
					  m_goingUp(true),
					  m_goingRight(false),
					  m_pointsOnCombo(1500),
					  m_explotionAnimationTime(1000),
					  m_explotionRemainingTime(0)
{
	m_tag = "Blg PLane";
	m_speed = Vector2D(1.4f, 1.75f);
	m_direction.setX(0);
	m_direction.setY(DIRECTION_UP);

	m_width = 128;// hardcodeado como valores iniciales antes de llamar a load
	m_height = 96;

	if (Random::getRange(-1, 1) >= 0)
		m_goingRight = true;

	generateBorderReturnOffset();

	m_health= 1000;
	m_pointOnKill = 1000;
	m_pointOnHit = 100;
	m_enemyWeapon = new EnemyBigBaseGun();
	setWeaponStats(4, 300, m_objectId, -1);

	m_shootingOffset = Vector2D(m_width/2, m_height/2);

}

BigPlane::~BigPlane()
{

}

void BigPlane::load(int x, int y, int width, int height, int textureID, int numFrames)
{
	 Enemy::load(x, y, width, height, textureID, numFrames);
	 m_hitRadius = m_width/2.5f;
}

void BigPlane::update()
{
	if (!m_dead)
	{
		if (m_exploting)
		{
			updateExplotionAnimation();
		}

		m_enemyWeapon->update();

		if (!m_dead && !m_dying)
		{
			//Si esta en la parte de abajo de la pantalla, Sube
			if (m_position.getY() >= (Game::Instance()->getGameHeight()/3) - m_height)
			{
				m_direction.setX(0);
				m_direction.setY(DIRECTION_UP);
				m_goingUp = true;
			}
			else
			{
				m_goingUp = false;
			}

			//Si llega a un borde, invierta la dirección en X
			if ((!m_goingRight) && (m_position.getX() <= m_borderReturnOffset))
			{
				flip();
			}
			if ((m_goingRight) && (m_position.getX() >= m_borderReturnOffset))
			{
				flip();
			}

			//Si no esta subiendo, tiende a moverse hacia la derecha o izquierda, segun corresponda
			if (!m_goingUp)
			{
				if (m_goingRight)
				{
					m_direction.setX(DIRECTION_RIGHT);
					m_direction.m_y += Random::getFloatRange(-0.25f, 0.25f);
					m_direction.normalize();
				}
				else
				{
					m_direction.setX(DIRECTION_LEFT);
					m_direction.m_y += Random::getFloatRange(-0.25f, 0.25f);
					m_direction.normalize();
				}
			}

			if (m_position.getY() < 50)
			{
				m_direction.m_y = DIRECTION_DOWN;
			}


			//Analiza si debe disparar
			if (!m_goingUp && isVisibleFromCamera())
			{
				int shootLuck = Random::getRange(0, 1000);
				if (shootLuck <= m_shootChance)
				{
					shoot();
				}
			}
		}
	}

	//Actualiza la posición y comunica a los clientes
	Enemy::update();

}

bool BigPlane::damage(int damageReceived, bool wasShoot,  Player* damager)
{
	bool killed = false;

	//Daña al avion
	m_health -= damageReceived;

	m_soundDirty = true;

	m_soundSendId = 52;

	if (damager && wasShoot)
	{
		Game::Instance()->addPointsToScore(m_pointOnHit, damager->getObjectId(), damager->getPlayerTeam().gameTeamID);
		damager->incrementHitsStats(1);

		PopUp* pointsPopUp = new PointsPopUp(damager->getObjectId(), m_pointOnHit);
		pointsPopUp->load(m_position.m_x + m_width/2, m_position.m_y + m_height/2, 96, 16, 104, 1);
		Game::Instance()->addPopUp(pointsPopUp);
	}
	if (damager)
	{
		updateKillerStats(damager->getObjectId(), damageReceived);
	}


	//Si lo mato suma puntos al player que lo mato
	if (m_health <= 0)
	{
		m_soundSendId = 53;
		m_dying = true;
		explote();
		if (canRetrievePoints() && damager)
		{
			int points = retrievePoints();
			Game::Instance()->addPointsToScore(points, damager->getObjectId(), damager->getPlayerTeam().gameTeamID);
			damager->incrementEnemiesKilledStats(1);

			PopUp* pointsPopUp = new PointsPopUp(damager->getObjectId(), points);
			pointsPopUp->load(m_position.m_x + m_width/2, m_position.m_y + m_height/2, 96, 16, 104, 1);
			Game::Instance()->addPopUp(pointsPopUp);
		}

		//Todos los aviones han sido matados
		calculateRewards();
	}

	return killed;
}

void BigPlane::kill()
{
	m_soundSendId = 53;
	m_dying = true;
	explote();
}

void BigPlane::dropLoot()
{
	PowerUp* drop = new ExtraPointsPU(1500);
	int posX = m_position.m_x + (m_width/2) - 24;
	int posY = m_position.m_y + (m_height/2) - 24;
	drop->load(posX, posY, 48, 48, 70, 1);
	CollitionHandler::Instance()->addPowerUp(drop);
	Game::Instance()->addPowerUp(drop);
}

void BigPlane::shoot()
{
	if (Game::Instance()->isPracticeMode())
		return;

	m_target = Game::Instance()->getRandomPLayerCenter();

	if (m_enemyWeapon)
	{
		Vector2D shootDirection = m_target - (m_shootingOffset + m_position);
		shootDirection.normalize();
		Vector2D shootPosition = m_shootingOffset + m_position;
		m_enemyWeapon->shoot(shootPosition, shootDirection);
	}
}

void BigPlane::updateKillerStats(int playerID, int damageDone)
{
	if (m_playerIDDamageDone.find(playerID) == m_playerIDDamageDone.end())
	{
		//nuevo killer
		m_playerIDDamageDone[playerID] = damageDone;
	}
	else
	{
		//ya habia matado a algun avion del a formacion
		m_playerIDDamageDone[playerID] = m_playerIDDamageDone[playerID] + damageDone;
	}
}

void BigPlane::calculateRewards()
{
	for (std::map<int,int>::iterator it = m_playerIDDamageDone.begin(); it != m_playerIDDamageDone.end(); ++it)
	{
		if (it->second >= m_kMaxHealth)
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
				break;
			}
		}
	}
}

void BigPlane::generateBorderReturnOffset()
{
	if (m_goingRight)
	{
		m_borderReturnOffset = Game::Instance()->getGameWidth() - m_width - Random::getRange(0, 50);
	}
	else
	{
		m_borderReturnOffset = Random::getRange(0, 50);
	}
}

void BigPlane::flip()
{
	if (m_goingRight)
	{
		m_goingRight = false;
		m_direction.setX(DIRECTION_LEFT);
		m_direction.setY(m_direction.m_y * (-1));
	}
	else
	{
		m_goingRight = true;
		m_direction.setX(DIRECTION_RIGHT);
		m_direction.setY(m_direction.m_y * (-1));
	}

	generateBorderReturnOffset();

}


void BigPlane::explote()
{
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	//hardcodeado por ahora
	/*m_numFrames = 12;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 31;*/
}

void BigPlane::updateExplotionAnimation()
{
	m_explotionRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_explotionAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	int lastRow = m_currentRow;

	m_currentFrame = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) % 4;
	m_currentRow = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) / 4;

	if (m_explotionRemainingTime <= 0)
	{
		m_dying = false;
		m_dead = true;
		m_exploting = false;
		dropLoot();
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}
