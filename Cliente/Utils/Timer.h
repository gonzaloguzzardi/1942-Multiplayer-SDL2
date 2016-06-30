/*
 * Timer.h
 *
 *  Created on: Apr 9, 2016
 *      Author: gonzalo
 */

#ifndef SRC_UTILS_TIMER_H_
#define SRC_UTILS_TIMER_H_

#include <iostream>
#include <cstdio>
#include <ctime>

class Timer
{

public:
	Timer();
	bool IsStarted();
	bool IsStopped();
	bool IsPaused();
	bool IsActive();
	void Pause();
	void Resume();
	void Stop();
	void Start();
	void Reset();
	clock_t GetTicks();

	clock_t m_startedAt;
	clock_t m_pausedAt;
	bool m_started;
	bool m_paused;

};


#endif /* SRC_UTILS_TIMER_H_ */
