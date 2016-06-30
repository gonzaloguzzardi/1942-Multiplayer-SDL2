/*
 * PointsPopUp.h
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#ifndef POPUPS_POINTSPOPUP_H_
#define POPUPS_POINTSPOPUP_H_

#include "PopUp.h"

class PointsPopUp: public PopUp
{
public:
    virtual void load(int x, int y, int width, int height, int textureID, int numFrames);

	PointsPopUp(int ownerID, int points);


private:
	int pointsToTextureID(int points);

	int m_points;
};



#endif /* POPUPS_POINTSPOPUP_H_ */
