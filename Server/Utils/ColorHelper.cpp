/*
 * ColorHelper.cpp
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#include "ColorHelper.h"

ColorHelper::ColorHelper()
{
}

ColorHelper::~ColorHelper()
{
}

Color ColorHelper::playerToColor(int playerID)
{
	Color playerColor;
	playerColor.value_A = 255;
	switch(playerID)
	{
	case 0:
		playerColor.value_R = 255;
		playerColor.value_G = 0;
		playerColor.value_B = 0;
		break;
	case 1:
		playerColor.value_R = 0;
		playerColor.value_G = 0;
		playerColor.value_B = 255;
		break;
	case 2:
		playerColor.value_R = 0;
		playerColor.value_G = 200;
		playerColor.value_B = 0;
		break;
	case 3:
		playerColor.value_R = 204;
		playerColor.value_G = 0;
		playerColor.value_B = 102;
		break;
	case 4:
		playerColor.value_R = 255;
		playerColor.value_G = 255;
		playerColor.value_B = 50;
		break;
	case 5:
		playerColor.value_R = 255;
		playerColor.value_G = 128;
		playerColor.value_B = 0;
		break;
	default:
		playerColor.value_R = 0;
		playerColor.value_G = 0;
		playerColor.value_B = 0;
	}
	return playerColor;
}

