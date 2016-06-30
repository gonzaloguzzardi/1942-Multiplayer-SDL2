/*
 * Player.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: gonzalo
 */

#include "Player.h"
#include "SecondaryShip.h"
#include "Singletons/CollisionHandler.h"
#include "Weapons/BulletsHandler.h"
#include "Weapons/Weapon.h"
#include "Weapons/PlayerWeapons/BasicWeapon.h"

using namespace std;

Player::Player() :  MoveableObject(),
					m_connected(true),
					m_doingFlip(false),
					m_hasSecondaryWeapons(false),
					m_flipAnimationTime(FLIP_ANIMATION_TIME),
					m_flipRemainingTime(0),
				    m_explotionAnimationTime(EXPLOTION_ANIMATION_TIME),
				    m_explotionRemainingTime(0),
					m_holdQuietTimer(100),
					m_currentHoldQuietTime(0),
				    m_respawningTimer(2500),
				    m_currentRespawningTime(0),
					m_lives(STARTING_LIVES),
					m_dead(false),
					m_dying(false),
					m_exploting(false),
					m_automaticMoving(false),
					m_invulnerable(false),
					m_respawning(false),
					m_health(STARTING_HEALTH),
					m_collisionDamage(100),
					m_movedByPlayer(false),
					m_autoMoveSpeed(1)
{
	m_score = Score();
	m_stageStats = Statistics();
	m_tag = "Player";
	m_layer = FOREGROUND;
	m_currentWeapon = new BasicWeapon();
	m_shootOffset = Vector2D(15, -5);
	m_destination = Vector2D (0, 0);

	m_leftSecondaryPlane = NULL;
	m_rightSecondaryPlane = NULL;
}

void Player::collision()
{

}

void Player::damage(int damageReceived)
{
	if (m_invulnerable)
	{
		return;
	}
	m_health -= damageReceived;
	if (m_health <= 0)
	{
		//Hacer explosion, setear dying en true, etc
		m_dying = true;
		explote();
		if (m_hasSecondaryWeapons)
		{
			if (m_leftSecondaryPlane)
				m_leftSecondaryPlane->setDead(true);
			if (m_rightSecondaryPlane)
				m_rightSecondaryPlane->setDead(true);
		}
	}
}

void Player::takeLife()
{
	if (m_lives <= 0)
	{
		//GAME OVER FOR THIS PLAYER
		m_dead = true;
	}
	else
	{
		// Take life and respawn
		--m_lives;
		Game::Instance()->sendPlayerDataUpdate(m_objectId);
		respawn();
	}
}

void Player::respawn()
{
	m_respawning = true;
	m_position.m_x = (Game::Instance()->getGameWidth()/2) - (m_width/2);
	m_position.m_y = (Game::Instance()->getGameHeight() *0.75f);
	m_direction.m_x = 0;
	m_direction.m_y = 0;
	m_textureID = m_objectId;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_health = STARTING_HEALTH;
	if (m_currentWeapon)
	{
		delete m_currentWeapon;
	}
	m_currentWeapon = new BasicWeapon();

	m_currentRespawningTime = m_respawningTimer;
	m_invulnerable = true;

	if (m_hasSecondaryWeapons)
	{
		if (m_leftSecondaryPlane)
		{
			m_leftSecondaryPlane->setDead(true);
		}
		if (m_rightSecondaryPlane)
		{
			m_rightSecondaryPlane->setDead(true);
		}
	}

}

void Player::doRespawningAnimation()
{
    if(m_alpha == 255)
    {
        m_alpha = 0;
    }
    else
    {
        m_alpha = 255;
    }
}

void Player::setShootingSpeed(int speed)
{
	if (m_currentWeapon)
	{
		m_currentWeapon->setBulletSpeed(speed);
	}
}
void Player::setShootingCooldown(int cooldown)
{
	if (m_currentWeapon)
	{
		m_currentWeapon->setShootingCooldown(cooldown);
	}
}

void Player::setWeapon(Weapon* weapon)
{
	if (!weapon)
		return;
	delete m_currentWeapon;
	m_currentWeapon = weapon;
}

void Player::setWeaponStats(int shootingSpeed, int shootingCooldown, int ownerID, int teamID)
{
	if (m_currentWeapon)
	{
		m_currentWeapon->setBulletSpeed(shootingSpeed);
		m_currentWeapon->setShootingCooldown(shootingCooldown);
		m_currentWeapon->setOwnerID(ownerID);
		m_currentWeapon->setOwnerTeam(teamID);
	}
}

void Player::StopFlipAnimation()
{
	//m_doingFlip = false;
	m_flipRemainingTime = 0;
	m_invulnerable = false;
}

void Player::load(int x, int y, int width, int height, int textureID, int numFrames)
{
    // Load comun. Inicializa variables
    MoveableObject::load(x, y, width, height, textureID, numFrames);

   // TextureManager::Instance()->load("Assets/Sprites/BlackShip.png", m_textureID, Game::Instance()->getRenderer());

    // Otras acciones de inicialización del Player más especificas

}

void Player::draw()
{
    TextureManager::Instance()->drawFrame(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height,
    										m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_NONE);
	//Nave::draw();    	int dy = 0;
}



void Player::update()
{
	//Workaround para que no aparezca invisible
	if (!m_movedByPlayer)
	{
		m_dirty = true;
	}

	if (m_exploting)
	{
		updateExplotionAnimation();
	}

	//printf("currentFrame: %d \n", m_currentFrame);
	//printf("doing flip: %d \n", m_doingFlip);
	if (m_doingFlip)
	{
		updateFlipAnimation();
	}

	if (m_respawning)
	{
		updateRespawningTimer();
	}


	if (m_automaticMoving)
	{
		moveTo(m_destination);
	}

	//Si esta girando no actualiza posicion
	if ((!m_doingFlip) || (m_automaticMoving))
	{
		MoveableObject::update();

		//FUNCIONALIDAD DE ARRASTRE, POR LAS DUDAS LO DEJO
		if (DRAG_PLAYER)
		{
			// si esta quieto Y no esta enl borde de abajo, lo empuja hacia abajo. Si esta desconectado se comporta de acuerdo a DRAG_DISCONNECTED_PLAYER
			if ((m_direction.getX() == 0) && (m_direction.getY() == 0) &&
				(((m_position.getY() + m_height) < Game::Instance()->getGameHeight() - 10)))
			{
				//Esta quieto. Iniciar Timer para arrastrar
				if (m_currentHoldQuietTime >= m_holdQuietTimer)
				{
					m_position.m_y += Game::Instance()->getScrollSpeed();
					m_dirty = true;
				}
				else
				{
					m_currentHoldQuietTime += GameTimeHelper::Instance()->deltaTime();
				}
			}
		}
	}
	m_currentWeapon->update();


	if (m_hasSecondaryWeapons)
	{
		updateSecondaryWeapons();
	}

	if (m_dirty)
	{
		sendDrawMessage(true);
		m_dirty = false;
	}

	m_direction.setX(0);
	m_direction.setY(0);
}

void Player::updateSecondaryWeapons()
{
	if (m_leftSecondaryPlane && m_leftSecondaryPlane->isDead() && m_leftSecondaryPlane->canRecycle() &&
			m_rightSecondaryPlane && m_rightSecondaryPlane->isDead()  && m_rightSecondaryPlane->canRecycle())
	{
		m_hasSecondaryWeapons = false;
		return;
	}

	if (m_leftSecondaryPlane)
	{
		if (m_leftSecondaryPlane->isDead() && m_leftSecondaryPlane->canRecycle())
		{
			delete m_leftSecondaryPlane;
			m_leftSecondaryPlane = NULL;
		}
		else
		{
			if (m_leftSecondaryPlane->isDead() == false)
			{
				m_leftSecondaryPlane->setPosition(Vector2D(m_position.m_x - m_width, m_position.m_y + m_height/2));

			}
			m_leftSecondaryPlane->update();
		}

	}

	if (m_rightSecondaryPlane)
	{
		if (m_rightSecondaryPlane->isDead() && m_rightSecondaryPlane->canRecycle())
		{
			delete m_rightSecondaryPlane;
			m_rightSecondaryPlane = NULL;
		}
		else
		{
			if (m_rightSecondaryPlane->isDead() == false)
			{
				m_rightSecondaryPlane->setPosition(Vector2D(m_position.m_x + m_width*1.5f, m_position.m_y + m_height/2));
			}
			m_rightSecondaryPlane->update();
		}

	}
}

void Player::createSecondaryShips()
{
	if (m_hasSecondaryWeapons)
	{
		if (!m_leftSecondaryPlane || (m_leftSecondaryPlane && m_leftSecondaryPlane->isDead()))
		{
			m_leftSecondaryPlane = new SecondaryShip();
			m_leftSecondaryPlane->load(m_position.m_x - m_width, m_position.m_y + m_height/2, 48, 48, 73, 1);
			CollitionHandler::Instance()->addSecondaryShip(m_leftSecondaryPlane);
		}

		if (!m_rightSecondaryPlane || (m_rightSecondaryPlane && m_rightSecondaryPlane->isDead()))
		{
			m_rightSecondaryPlane = new SecondaryShip();
			m_leftSecondaryPlane->load(m_position.m_x + m_width, m_position.m_y + m_height/2, 48, 48, 73, 1);
			CollitionHandler::Instance()->addSecondaryShip(m_rightSecondaryPlane);
		}
	}
	else
	{
		m_leftSecondaryPlane = new SecondaryShip();
		m_leftSecondaryPlane->load(m_position.m_x - m_width, m_position.m_y + m_height/2, 48, 48, 73, 1);
		CollitionHandler::Instance()->addSecondaryShip(m_leftSecondaryPlane);

		m_rightSecondaryPlane = new SecondaryShip();
		m_leftSecondaryPlane->load(m_position.m_x + m_width, m_position.m_y + m_height/2, 48, 48, 73, 1);
		CollitionHandler::Instance()->addSecondaryShip(m_rightSecondaryPlane);
	}

	m_hasSecondaryWeapons = true;

}

void Player::updateFlipAnimation()
{
	m_flipRemainingTime -= GameTimeHelper::Instance()->deltaTime();
	int step = m_flipAnimationTime / m_numFrames;
	int lastFrame = m_currentFrame;
	m_currentFrame = (m_flipAnimationTime - m_flipRemainingTime) / step;

	if (m_flipRemainingTime <= 0)
	{
		m_currentFrame = 0;
		m_doingFlip = false;
		m_invulnerable = false;
	}

	if (lastFrame != m_currentFrame)
		m_dirty = true;
}

void Player::explote()
{
	m_exploting = true;
	m_explotionRemainingTime = m_explotionAnimationTime;
	//hardcodeado por ahora
	m_numFrames = 14;
	m_currentFrame = 0;
	m_currentRow = 0;
	m_textureID = 41;
	m_soundDirty = true;

    m_soundSendId = 53;
}

void Player::updateExplotionAnimation()
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
		m_exploting = false;
		takeLife();
	}

	if ((lastFrame != m_currentFrame) || (lastRow != m_currentRow))
	{
		m_dirty = true;
	}
}

void Player::updateRespawningTimer()
{
	m_currentRespawningTime -= GameTimeHelper::Instance()->deltaTime();
	if (m_currentRespawningTime <= 0)
	{
		m_respawning = false;
		if (Game::Instance()->isPracticeMode() == false)
		{
			m_invulnerable = false;
			m_alpha = 255;
		}
	}
	else
	{
		doRespawningAnimation();
	}
	m_dirty = true;
}


void Player::clean()
{
    MoveableObject::clean();

    if (m_currentWeapon)
    	delete m_currentWeapon;
}

void Player::handleInput(InputMessage inputMsg)
{
	if (!Game::Instance()->isLevelStarted() || (Game::Instance()->isFinishingLevel()))
		return;

	if (!m_movedByPlayer)
	{
		m_movedByPlayer = true;
	}

	if (inputMsg.togglePracticeMode == 1)
	{
		bool practiceMode = (!(Game::Instance()->isPracticeMode()));
		Game::Instance()->setPracticeMode(practiceMode);
	}

    if(!m_dead && !m_dying)
    {
    	//Si esta girando no admite inputs de disparo ni movimiento
    	if(m_doingFlip)
    	{
    		return;
    	}

        // handle keys
    	int dy = 0;
    	int dx = 0;
        if ((inputMsg.buttonUp == 1) && (m_position.getY() > 0))
        {
            --dy;
            m_currentHoldQuietTime = 0;
            m_dirty = true;
        }
        if ((inputMsg.buttonDown == 1) && ((m_position.getY() + m_height) < Game::Instance()->getGameHeight() - 10))
        {
        	++dy;
        	m_currentHoldQuietTime = 0;
            m_dirty = true;
        }

        if ((inputMsg.buttonLeft == 1)	&& m_position.getX() > 0)
        {
        	--dx;
        	m_currentHoldQuietTime = 0;
            m_dirty = true;
        }
        if ((inputMsg.buttonRight == 1) && ((m_position.getX() + m_width) < Game::Instance()->getGameWidth()))
        {
        	++dx;
        	m_currentHoldQuietTime = 0;
            m_dirty = true;
        }
        m_direction.setX(dx);
        m_direction.setY(dy);
        //Se mueve a velocidades constantes. Evita que vaay a mayot velocidad en diagonal
        m_direction.normalize();

        if (inputMsg.buttonShoot)
        {
        	shoot();
            m_dirty = true;
        }

        if (inputMsg.buttonRoll)
        {
        	if (!m_doingFlip)
        	{
        		startFlipAnimation();
        	}
            m_dirty = true;
        }
    }
}

void Player::shoot()
{
	Vector2D shootPosition;
	bool didShoot = m_currentWeapon->shoot(Vector2D(m_position.getX() + m_shootOffset.getX(), m_position.getY() + m_shootOffset.getY()),
							Vector2D(0, DIRECTION_UP));
	if (didShoot)
	{
		shootWithSecondaryWeapons();
       	m_stageStats.incrementShoots(1);
    	m_soundDirty = true;
    	m_soundSendId = 51;
	}
}

void Player::shootWithSecondaryWeapons()
{
	if (m_hasSecondaryWeapons)
	{
		if (m_leftSecondaryPlane && !m_leftSecondaryPlane->isDead())
		{
			m_leftSecondaryPlane->shoot(Vector2D(m_leftSecondaryPlane->getPosition().m_x + 16, m_leftSecondaryPlane->getPosition().m_y),
										Vector2D(0, DIRECTION_UP), m_currentWeapon->getBulletSpeed(), m_objectId, playerGameTeam.gameTeamID);
		}

		if (m_rightSecondaryPlane && !m_rightSecondaryPlane->isDead())
		{
			m_rightSecondaryPlane->shoot(Vector2D(m_rightSecondaryPlane->getPosition().m_x + 16, m_rightSecondaryPlane->getPosition().m_y),
										Vector2D(0, DIRECTION_UP), m_currentWeapon->getBulletSpeed(), m_objectId, playerGameTeam.gameTeamID);
		}
	}
}

void Player::startFlipAnimation()
{
		m_flipRemainingTime = m_flipAnimationTime;
		m_doingFlip = true;
		m_invulnerable = true;
}

void Player::addPoints(int points)
{
	m_score.addPoints(points);
	m_stageStats.incrementPoints(points);

	ScoreMessage scoreMsg;
	scoreMsg.playerID = m_objectId;
	scoreMsg.pointsacquire = points;
	scoreMsg.teamID = playerGameTeam.gameTeamID;
	scoreMsg.somethingElse = 0;

	Game::Instance()->sendScoreToClients(scoreMsg);
}

const int Player::getScore()
{
	return m_score.getScore();
}

void Player::reset()
{
	m_connected = true;
	m_doingFlip = false;
	m_flipAnimationTime = FLIP_ANIMATION_TIME;
	m_flipRemainingTime = 0;
    m_explotionAnimationTime = EXPLOTION_ANIMATION_TIME;
    m_explotionRemainingTime = 0;
	m_currentHoldQuietTime = 0;
	m_dead = false;
	m_dying = false;
	m_canRecycle = false;
	m_exploting = false;
	m_health = STARTING_HEALTH;
	m_lives = STARTING_LIVES;
	m_movedByPlayer = false;
	m_textureID = m_objectId;
	m_currentFrame = 0;
	m_currentRow = 0;

	m_stageStats.reset();
	m_score.reset();

	if (m_hasSecondaryWeapons)
	{
		if (m_leftSecondaryPlane)
		{
			m_leftSecondaryPlane->setDead(true);
		}
		if (m_rightSecondaryPlane)
		{
			m_rightSecondaryPlane->setDead(true);
		}
	}
}

void Player::moveAutomatic(const Vector2D& destination, int speed)
{
	m_automaticMoving = true;
	m_destination = destination;
	m_autoMoveSpeed = speed;
}

void Player::moveTo(const Vector2D& destination)
{
	//en movimiento automatico no puede recibir daño
	m_invulnerable = true;
	m_direction.setX(m_destination.m_x - m_position.m_x);
	m_direction.setY(m_destination.m_y - m_position.m_y);
	m_direction.normalize();
	float nextPositionX = m_position.m_x + (m_direction.getX() * m_autoMoveSpeed);
	float nextPositionY = m_position.m_y + (m_direction.getY() * m_autoMoveSpeed);
	if ((((m_position.m_x < m_destination.m_x) && (nextPositionX >= m_destination.m_x )) ||
		((m_position.m_x > m_destination.m_x) && (nextPositionX <= m_destination.m_x ))) &&
		(((m_position.m_y < m_destination.m_y) && (nextPositionY >= m_destination.m_y )) ||
		((m_position.m_y > m_destination.m_y) && (nextPositionY <= m_destination.m_y ))))
	{
		m_position.m_x = m_destination.m_x;
		m_position.m_y = m_destination.m_y;
		m_automaticMoving = false;
	}
	else
	{
		//mueve al personaje
		m_position.m_x += (m_direction.getX() * m_autoMoveSpeed);
		m_position.m_y += (m_direction.getY() * m_autoMoveSpeed);
	}
	if ((m_position.m_x == m_destination.m_x) && (m_position.m_y == m_destination.m_y))
	{
		m_automaticMoving = false;
		m_invulnerable = false;
	}
	//Hardcodeado para que se mueva automaticamente sin direccion
	m_dirty = true;
	m_direction.setX(0);
	m_direction.setY(0);
}


void Player::sendDrawMessage(bool isAlive)
{
	DrawMessage drawMsg;
	drawMsg.connectionStatus = m_connected;
	drawMsg.unused1 = false;
	drawMsg.alive = isAlive;

	if(m_soundDirty)
	{
		drawMsg.hasSound = true;
		m_soundDirty = false;

	}else
	{
		drawMsg.hasSound = false;
	}

	drawMsg.objectID = m_objectId;
	drawMsg.layer = m_layer;
	drawMsg.soundID = m_soundSendId;
	drawMsg.column = m_currentFrame;
	drawMsg.row = m_currentRow;
	drawMsg.posX = m_position.getX();
	drawMsg.posY = m_position.getY();
	drawMsg.textureID = m_textureID;
	drawMsg.angle = static_cast<float>(m_angle);
	drawMsg.alpha = m_alpha;
	drawMsg.vacio = 0;

	if (USE_DRAWMESSAGE_PACKAGING)
	{
		Game::Instance()->addToPackage(drawMsg);
	}
	else
	{
		Game::Instance()->sendToAllClients(drawMsg);
	}
}
