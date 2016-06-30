/*
 * MovCircular.h
 *
 *  Created on: Jun 4, 2016
 *      Author: matias
 */

#ifndef MOVCIRCULAR_H_
#define MOVCIRCULAR_H_
#include "Movement.h"
#include <math.h>
#include "../Singletons/GameTimeHelper.h"

class MovCircular : public Movement{
public:
	MovCircular(int angle, int amp);
	void update();
	virtual ~MovCircular();

};


#endif /* MOVCIRCULAR_H_ */
