/*
 * ColorHelper.h
 *
 *  Created on: Jun 19, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_COLORHELPER_H_
#define UTILS_COLORHELPER_H_

#include "TiposDefinidos.h"

class ColorHelper
{
public:
	ColorHelper();
	~ColorHelper();
	static Color playerToColor(int playerID);
};



#endif /* UTILS_COLORHELPER_H_ */
