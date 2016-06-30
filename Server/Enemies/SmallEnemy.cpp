/*
 * SmallEnemy.cpp
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#include "SmallEnemy.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "../Weapons/EnemyWeapons/EnemyBaseGun.h"
#include "../Player.h"
#include "../PopUps/PointsPopUp.h"


SmallEnemy::SmallEnemy() :Enemy(),
						m_shootChance(5),
						m_fleeing(false),
						m_flipping(false),
						m_flipAnimationTime(500),
						m_flipRemainingTime(0),
					    m_explotionAnimationTime(1000),
					    m_explotionRemainingTime(0)
{
	m_tag = "Small Plane";
	m_speed = Vector2D(2.5f, 2.5f);
	float randomXDirection = Random::getFloatRange(-0.5f, 0.5f);
	m_direction.setX(randomXDirection);
	m_direction.setY(1);

	//angulo inicial
	//angulo en radianes = arctan(y/x)
	//angulo en grados = angulo en radianes * 180/pi = angulo en gradianes * 57.295645531f
	//luego se le suma un offser debido al giro del sprite
	m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 270;

	m_health = 100;
	m_pointOnKill = 50;
	m_enemyWeapon = new EnemyBaseGun();
	setWeaponStats(4, 250, m_objectId, -1);

	m_returnYOffset = Random::getRange(0, Game::Instance()->getGameHeight()/2);
}

SmallEnemy::~SmallEnemy()
{

}

void SmallEnemy::update()
{

	//Si esta huyendo y ya no es visible, lo elimina
	if ((m_fleeing) && (!isVisibleFromCamera()))
	{
		//todo Setea en dead para limpiarlo despues.
		//Cuando tengamos el contenedor con enemigos, al final del frame deberiamos loopear y eliminar los enemigos muertos
		m_dead = true;
	}

	if (m_exploting)
	{
		updateExplotionAnimation();
	}

	m_enemyWeapon->update();

	if (!m_dead && !m_dying)
	{
		//Si esta girando, actualiza el frame a mostrar
		if (m_flipping)
		{
			updateFlipAnimation();
		}

		//Controla que se mueva adentro de la pantalla
		if ((m_position.getX() <= 0 ) && (!m_fleeing))
		{
			float randomXDirection = Random::getFloatRange(0, 1);
			m_direction = Vector2D(randomXDirection, m_direction.m_y);
			m_direction.normalize();
			m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 270;
		}

		if ((m_position.getX() >= Game::Instance()->getGameWidth() - m_width) && (!m_fleeing))
		{
			float randomXDirection = Random::getFloatRange(-1, 0);
			m_direction = Vector2D(randomXDirection, m_direction.m_y);
			m_direction.normalize();
			m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 270;
		}


		//Retorna si llega suficientemente abajo
		if (m_position.getY() >= Game::Instance()->getGameHeight() - m_returnYOffset)
		{
			retire();
		}

		//Analiza si debe disparar
		int shootLuck = Random::getRange(0, 1000);
		if ((shootLuck <= m_shootChance) && (!m_flipping) && isVisibleFromCamera())
		{
			//dispara
			shoot();

			//retrocede con un 25% de probabilidad luego del disparo
			int fleeChance = Random::getRange(0, 100);
			if ((fleeChance <= 25) && (m_position.getY() >= Game::Instance()->getGameHeight()/2))
			{
				retire();
			}
		}
	}
	//Actualiza la posición y comunica a los clientes
	Enemy::update();

}

bool SmallEnemy::damage(int damageReceived, bool wasShoot,  Player* damager)
{
	bool killed = false;
	m_health -= damageReceived;
	if (damager && wasShoot)
		damager->incrementHitsStats(1);

	m_soundSendId = true;
	m_soundSendId = 52;
	if (m_health <= 0)
	{
		m_soundSendId = 53;
		m_dying = true;
		explote();
		if (canRetrievePoints() && damager)
		{
			int points = retrievePoints();
			Game::Instance()->addPointsToScore(points, damager->getObjectId(), damager->getPlayerTeam().gameTeamID);

			PopUp* pointsPopUp = new PointsPopUp(damager->getObjectId(), points);
			pointsPopUp->load(m_position.m_x + m_width/2, m_position.m_y + m_height/2, 96, 16, 104, 1);
			Game::Instance()->addPopUp(pointsPopUp);


			damager->incrementEnemiesKilledStats(1);
		}
	}
	return killed;
}

void SmallEnemy::kill()
{
	m_soundSendId = 53;
	m_dying = true;
	explote();
}

void SmallEnemy::shoot()
{
	if (Game::Instance()->isPracticeMode())
		return;

	m_target = Game::Instance()->getRandomPLayerCenter();

	if (m_enemyWeapon)
	{
		Vector2D shootDirection = m_target - m_position;
		shootDirection.normalize();
		m_enemyWeapon->shoot(m_position, shootDirection);
	}
}

void SmallEnemy::retire()
{
	m_fleeing = true;
	m_direction = Vector2D(m_direction.m_x, -1);
	m_direction.normalize();
	m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 270;

	m_flipping = true;
	m_flipRemainingTime = m_flipAnimationTime;

}

void SmallEnemy::updateFlipAnimation()
{
	m_flipRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_flipAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	m_currentFrame = (m_flipAnimationTime - m_flipRemainingTime) / step;

	if (m_flipRemainingTime <= 0)
	{
		m_currentFrame = m_numFrames;
		m_flipping = false;
	}

	if (lastFrame != m_currentFrame)
	{
		m_dirty = true;
	}
}

void SmallEnemy::explote()
{
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	//hardcodeado por ahora
	m_numFrames = 29;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 40;
}

void SmallEnemy::updateExplotionAnimation()
{
	m_explotionRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_explotionAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	int lastRow = m_currentRow;

	m_currentFrame = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) % 10;
	m_currentRow = ((m_explotionAnimationTime - m_explotionRemainingTime) / step) / 10;

	if (m_explotionRemainingTime <= 0)
	{
		m_dying = false;
		m_dead = true;
		m_exploting = false;
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}
