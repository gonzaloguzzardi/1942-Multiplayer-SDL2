/*
 * Player.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: gonzalo
 */

#include "Player.h"

using namespace std;

Player::Player() :  MoveableObject(),
					m_controllable(true),
					m_doingFlip(false),
					m_dead(false),
					m_dying(false)
{
	m_tag = "Player";
	m_layer = FOREGROUND;
}

Player::Player(bool canControl) :  MoveableObject(),
									m_dead(false),
									m_dying(false)
{
	m_controllable = canControl;
	m_tag = "Player";
	m_layer = FOREGROUND;
}

void Player::collision()
{

}

void Player::load(int x, int y, int width, int height, int textureID, int numFrames)
{
    // Load comun. Inicializa variables
    MoveableObject::load(x, y, width, height, textureID, numFrames);

    TextureManager::Instance()->load("Assets/Sprites/BlackShip.png", m_textureID, Game::Instance()->getRenderer());

    // Otras acciones de inicialización del Player más especificas

}

void Player::draw()
{
    TextureManager::Instance()->drawFrame(m_textureID, m_position.getX(), m_position.getY(), m_width, m_height,
    										m_currentRow, m_currentFrame, Game::Instance()->getRenderer(), m_angle, m_alpha, SDL_FLIP_NONE);
	//Nave::draw();
}

void Player::update()
{
	MoveableObject::update();
	//Probar valores para animacion
	//m_currentFrame = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));
}


void Player::clean()
{
    MoveableObject::clean();
}

void Player::handleInput()
{
	if (!m_controllable || m_dead || m_dying || Game::Instance()->isReseting())
		return;

	InputMessage inputMsg;

	inputMsg.objectID = getObjectId();
	inputMsg.buttonUp=0;
	inputMsg.buttonDown=0;
	inputMsg.buttonRight=0;
	inputMsg.buttonLeft=0;
	inputMsg.buttonShoot=0;
	inputMsg.buttonRoll=0;

	bool dirty = false;

	// handle keys
	//Espacio
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
	{
		inputMsg.buttonShoot = 1;
		dirty = true;
	}
	//Movimiento Hacia Arriba
	if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))))
	{
		inputMsg.buttonUp = 1;
		dirty = true;
	}
	//Movimiento Hacia Abajo
	if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))))
	{
		inputMsg.buttonDown = 1;
		dirty = true;
	}
	//Movimiento Hacia la Izquierda
	if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))))
	{
		inputMsg.buttonLeft = 1;
		dirty = true;
	}
	//Movimiento Hacia La Derecha
	if (((InputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) || (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))))
	{
		inputMsg.buttonRight = 1;
		dirty = true;
	}
	//Vuelta
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
	{
		inputMsg.buttonRoll = 1;
		dirty = true;
	}

	//Toggle Practice Mode
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_F1))
	{
		inputMsg.togglePracticeMode = 1;
		dirty = true;
	}
	//Toggle AllSounds Mode
		if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_F2))
		{
			Game::Instance()->setAlltalk(!Game::Instance()->getAlltalk());
		}
	//RESET GAME
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_R))
	{
		if (!Game::Instance()->isReseting())
		{
			SDL_FlushEvent(SDL_KEYDOWN);
			Game::Instance()->setReseting(true);
			NetworkMessage netMsg;
			netMsg.msg_Code[0] = 'r';
			netMsg.msg_Code[1] = 's';
			netMsg.msg_Code[2] = 't';
			netMsg.msg_Length =  MESSAGE_LENGTH_BYTES + MESSAGE_CODE_BYTES;

			Game::Instance()->sendNetworkMsg(netMsg);
		}
	}
	//Abort
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_X))
	{
		Game::Instance()->quit();
		dirty = false;
	}

	if (dirty)
	{
		//printf("Enviando Input del objeto %d \n", getObjectId());
		Game::Instance()->sendInputMsg( inputMsg);
	}
}
