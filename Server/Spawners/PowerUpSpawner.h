/*
 * PowerUpSpawner.h
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#ifndef SPAWNERS_POWERUPSPAWNER_H_
#define SPAWNERS_POWERUPSPAWNER_H_

#include "../Utils/TiposDefinidos.h"
#include <vector>
class PowerUp;
class TextureHelper;

class PowerUpSpawner
{
public:
	PowerUpSpawner();
	~PowerUpSpawner();
	void feed(std::vector<Powerup> powerUps, int stageSize);
	void update(int stagePosition);
	void reset();
	void clean();

private:

	std::vector<PowerUpSpawnInfo> m_powerUpsToSpawn;

	TextureHelper* m_textureHelper;
	int m_stageSize;

	void addPowerUp(Powerup powerUpInfo);
	int getRandomPointsValue();

};


#endif /* SPAWNERS_POWERUPSPAWNER_H_ */
