/*
 * DrawObject.h
 *
 *  Created on: May 1, 2016
 *      Author: matias
 */

#ifndef DRAWOBJECT_H_
#define DRAWOBJECT_H_
#include "Game.h"
#include "GameObject.h"
#include "Singletons/TextureManager.h"

class DrawObject : public GameObject{
public:
	DrawObject();
	virtual ~DrawObject();
	virtual void load(int x, int y, int width, int height, int textureID, int numFrames);
	virtual void load(int x, int y, int textureID);
	virtual void draw();
	virtual void clean();
	virtual void update(){}
	void setPosition(const Vector2D& position) {m_position = position;}
	void setCurrentRow(int currentRow) {m_currentRow = currentRow;}
	void setCurrentFrame(int currentFrame) {m_currentFrame = currentFrame;}
};

#endif /* DRAWOBJECT_H_ */
