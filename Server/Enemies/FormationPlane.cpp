/*
 * FormationPlane.cpp
 *
 *  Created on: Jun 11, 2016
 *      Author: gonzalo
 */

#include "FormationPlane.h"

#include "../Game.h"
#include "../Vector2D.h"
#include "../Weapons/EnemyWeapons/EnemyBaseGun.h"
#include "../Player.h"
#include "Formation.h"
#include "../PopUps/PointsPopUp.h"


FormationPlane::FormationPlane(Formation* parent) :Enemy(),
						m_shootChance(5),
						m_doingCircleAnimation(false),
						m_disruptingCircle(false),
						m_fleeing(false),
					    m_explotionAnimationTime(1000),
					    m_explotionRemainingTime(0)
{
	m_tag = "Formation Plane";
	m_formationParent = parent;

	m_speed = Vector2D(1.5f, 1.5f);
	//angulo inicial
	//angulo en radianes = arctan(y/x)
	//angulo en grados = angulo en radianes * 180/pi = angulo en gradianes * 57.295645531f
	//luego se le suma un offser debido al giro del sprite
	m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 90;

	m_health= 100;
	m_pointOnKill = 100;
	m_enemyWeapon = new EnemyBaseGun();
	setWeaponStats(4, 400, m_objectId, -1);

}

FormationPlane::~FormationPlane()
{

}

void FormationPlane::update()
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

	if (!m_dead && !m_dying && isVisibleFromCamera())
	{
		//Analiza si debe disparar
		int shootLuck = Random::getRange(0, 1000);
		if (shootLuck <= m_shootChance)
		{
			shoot();
		}
	}
	//Actualiza la posición y comunica a los clientes
	Enemy::update();

}

void FormationPlane::updateAngle()
{
	m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 90;
}

bool FormationPlane::damage(int damageReceived, bool wasShoot,  Player* damager)
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
			damager->incrementEnemiesKilledStats(1);

			PopUp* pointsPopUp = new PointsPopUp(damager->getObjectId(), points);
			pointsPopUp->load(m_position.m_x + m_width/2, m_position.m_y + m_height/2, 96, 16, 104, 1);
			Game::Instance()->addPopUp(pointsPopUp);
		}
		if (m_formationParent && damager)
			m_formationParent->updateKillerStats(damager->getObjectId());
	}
	return killed;
}

void FormationPlane::kill()
{
	m_soundSendId = 53;
	m_dying = true;
	explote();
}

void FormationPlane::shoot()
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

void FormationPlane::retire()
{
	m_fleeing = true;
	m_direction = Vector2D(m_direction.m_x, -1);
	m_direction.normalize();
	m_angle = (atan2f(m_direction.m_y, m_direction.m_x)* 57.295645531f) + 90;
}

void FormationPlane::explote()
{
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	//hardcodeado por ahora
	m_numFrames = 29;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 40;
}

void FormationPlane::updateExplotionAnimation()
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
