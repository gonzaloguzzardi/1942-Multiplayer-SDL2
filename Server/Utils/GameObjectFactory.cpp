/*
 * GameObjectFactory.cpp
 *
 *  Created on: May 10, 2016
 *      Author: gonzalo
 */

#include "GameObjectFactory.h"

GameObject* GameObjectFactory::createGameObject(const std::string& gameObjectID)
{
	GameObject* go;
	if (gameObjectID.compare("isla") == 0)
	{
		go = new Island();
		return go;
	}
	if (gameObjectID.compare("piedra") == 0)
	{
		go = new Island();
		return go;
	}

	if (gameObjectID.compare("Portaviones") == 0)
	{
		go = new Portaviones();
		return go;
	}
	if (gameObjectID.compare("default") == 0)
	{
		go = new Island();
		return go;
	}

	delete go;
	return nullptr;

}
