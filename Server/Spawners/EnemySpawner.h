/*
 * EnemySpawner.h
 *
 *  Created on: Jun 12, 2016
 *      Author: gonzalo
 */

#ifndef SPAWNERS_ENEMYSPAWNER_H_
#define SPAWNERS_ENEMYSPAWNER_H_

#include "../Utils/TiposDefinidos.h"
#include <vector>
#include <string>
class Enemy;
class TextureHelper;

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();
	void feed(std::vector<Enemigo> enemies, int stageSize);
	void update(int stagePosition);
	void reset();
	void clean();

private:

	std::vector<EnemySpawnInfo> m_enemiesToSpawn;
	TextureHelper* m_textureHelper;
	int m_stageSize;

	void addEnemy(Enemigo enemyInfo);

};


#endif /* SPAWNERS_ENEMYSPAWNER_H_ */
