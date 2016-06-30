/*
 * GameObjectFactory.h
 *
 *  Created on: May 10, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_GAMEOBJECTFACTORY_H_
#define UTILS_GAMEOBJECTFACTORY_H_

#include "../GameObject.h"
#include "../Background/Island.h"
#include "../Background/Portaviones.h"

#include <string>

class GameObjectFactory
{
public:
	static GameObject* createGameObject(const std::string& gameObjectID);

};


#endif /* UTILS_GAMEOBJECTFACTORY_H_ */
