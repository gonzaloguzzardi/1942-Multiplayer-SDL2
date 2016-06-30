/*
 * Level.h
 *
 *  Created on: May 9, 2016
 *      Author: gonzalo
 */

#ifndef BACKGROUND_LEVEL_H_
#define BACKGROUND_LEVEL_H_

#include "../Game.h"
#include "../GameObject.h"
#include "Portaviones.h"
#include "Background.h"
#include "Island.h"
#include "../Utils/Parser/ParserNivel.h"
#include "../Utils/TextureHelper.h"
#include "../Utils/GameObjectFactory.h"
#include <vector>
#include <sstream>
#include <string>

class Level
{
public:
	Level();
	~Level();

	void loadFromXML(const std::string& xmlFileName);
	void initialize(int backgroundTextureID, int levelWidth, int levelHeigth, float scrollSpeed);
	void addObject(GameObject* gameObject, int x, int y);
	//void paintBackground();
	void update();
	void clean();

	void reset();
	void resetPositions();
	void loadLevel(int level);

	void scrollToNextStage();
	bool isScrollingToNextStage() { return m_scrollingToNextStage; }

	void setScrollSpeed(int speed) { m_scrollSpeed = speed; }
	int getVirtualPosition() { return m_virtualPosition; }
	int getLevelHeight() { return m_levelHeight; }
	int getScrollSpeed() { return m_scrollSpeed; }

private:

	bool m_scrollingToNextStage;
	int m_nextStageScrollAmount;

	int m_virtualPosition; // Posicion en Y virtual, tiene el cero en la esquina de abajo e incrementa hacia arriba

	int m_backgroundTextureID;
	int m_levelWidth;
	int m_levelHeight;
	int m_scrollSpeed;
	std::vector<GameObject*> m_levelObjects;

	ParserNivel* m_parserNivel;
	TextureHelper* m_textureHelper;
};



#endif /* BACKGROUND_LEVEL_H_ */
