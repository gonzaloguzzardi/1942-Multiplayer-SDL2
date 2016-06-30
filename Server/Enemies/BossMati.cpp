/*
 * SmallEnemy.cpp
 *
 *  Created on: May 28, 2016
 *      Author: gonzalo
 */

#include "BossMati.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "../Weapons/EnemyWeapons/BossBaseGun.h"
#include "../Movements/MovCircular.h"

BossMati::BossMati() :Enemy(),
						m_fleeing(false),
						m_flipping(false),
						m_flipAnimationTime(1000),
						m_flipRemainingTime(0),
					    m_explotionAnimationTime(1000),
					    m_explotionRemainingTime(0)
{
	m_speed = Vector2D(1.85f, 1.85f);
	m_patronCirc = new MovCircular(0,70);
	float randomXDirection = Random::getFloatRange(-0.6f, 0.6f);
	m_direction.setX(0);
	m_direction.setY(0);

	lastX=0;
	lastY=0;
	delay=10;
	m_health= 5000;
	m_enemyWeapon = new BossBaseGun();
	setWeaponStats(2, 250, m_objectId, -1);

	m_returnYOffset = Random::getRange(0, Game::Instance()->getGameHeight()/2);

}

BossMati::~BossMati()
{

}

void BossMati::update()
{

	//Si esta huyendo y ya no es visible, lo elimina
	if ((!isVisibleFromCamera()))
	{
		//todo Setea en dead para limpiarlo despues.
		//Cuando tengamos el contenedor con enemigos, al final del frame deberiamos loopear y eliminar los enemigos muertos
		m_dead = true;
	}
	m_enemyWeapon->update();
	delay--;
	if (0>delay){
		m_patronCirc->update();
		m_position.setX(m_position.getX()-lastX+m_patronCirc->getX());
		m_position.setY(m_position.getY()-lastY+m_patronCirc->getY());
		lastX=m_patronCirc->getX();
		lastY=m_patronCirc->getY();
		delay = 0;
	}
	if (m_dying)
	{
		updateExplotionAnimation();
	}

	if (!m_dead && !m_dying)
	{
		//Analiza si debe disparar
		int shootLuck = Random::getRange(0, 1000);
		if ((shootLuck <= SHOOT_CHANCE))
		{
			//dispara
			shoot();


		}
	}

	//Actualiza la posición y comunica a los clientes
	Enemy::update();

}

void BossMati::damage(int damageReceived)
{
	m_health -= damageReceived;

	if (m_health <= 0)
	{
		//Hacer explosion, setear dying en true, etc
		m_explotionRemainingTime = m_explotionAnimationTime;
		//hardcodeado por ahora
		m_numFrames = 29;
		m_currentFrame = 0;
		m_currentRow = 0;
		m_textureID = 40;
		m_dying = true;
	}
}

void BossMati::shoot()
{
	m_target = Game::Instance()->getRandomPLayerCenter();

	if (m_enemyWeapon)
	{
		Vector2D shootDirection;
		shootDirection.setX(m_target.m_x - m_position.m_x);
		shootDirection.setY(m_target.m_y - m_position.m_y);
		shootDirection.normalize();
		m_enemyWeapon->shoot(m_position, shootDirection);
	}
}

void BossMati::updateExplotionAnimation()
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
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}
