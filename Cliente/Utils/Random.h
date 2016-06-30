/*
 * Random.h
 *
 *  Created on: Apr 22, 2016
 *      Author: gonzalo
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include <time.h>
#include <stdlib.h>

class Random
{
public:
	static void seed(unsigned int seed)
	{
		srand(seed);
	}

	static void initialize()
	{
		srand (static_cast <unsigned> (time(0)));
	}

	static int getRange (int base, int top)
	{
		return ((rand() % ( top + 1 - base )) + base);
	}

	static float getFloatRange (float base, float top)
	{
		return (base + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(top-base))));
	}

};



#endif /* RANDOM_H_ */
