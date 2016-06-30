/*
 * Score.h
 *
 *  Created on: Jun 8, 2016
 *      Author: gonzalo
 */

#ifndef SCORE_H_
#define SCORE_H_

class Score
{

public:

	Score(): m_currentScore(0)
	{
	}
	~Score()
	{

	}

	void addPoints(int points)
	{
		m_currentScore += points;
		if (m_currentScore < 0)
			m_currentScore = 0;
	}

	void reset()
	{
		m_currentScore = 0;
	}

	const int getScore() { return m_currentScore; }

private:
	int m_currentScore;

};

#endif /* SCORE_H_ */
