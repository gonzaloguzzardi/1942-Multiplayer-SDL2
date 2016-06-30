/*
 * MovCircular.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: matias
 */

#include "MovCircular.h"

MovCircular::MovCircular(int angle, int amp): Movement(){
	 m_angle = angle;
	 m_amplitud = amp;
	 x=0;
	 y=0;
}

MovCircular::~MovCircular() {
	// TODO Auto-generated destructor stub
}
void MovCircular::update(){

		   m_angle += 0.01745329251f;
		   x = (float) (((float) cosf(m_angle) * m_amplitud));
		   y = (float) (((float) sinf(m_angle) * m_amplitud));


}
