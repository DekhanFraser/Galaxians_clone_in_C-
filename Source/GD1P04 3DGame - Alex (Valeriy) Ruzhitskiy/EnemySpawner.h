//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : EnemySpwaner.h
//  Description: EnemySpawner gameobject header.
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#pragma once

#include "GameObject.h"
#include "ResourceManager.h"
#include "Enemy.h"

class Engine;

class EnemySpawner : public GameObject
{
public:
	EnemySpawner(Engine *engine);
	~EnemySpawner();
	void Init(int level);
	void Update(GLfloat dt);
	void SpawnEnemy(Behavior behavior);
	void SpawnSuicider();
	void SpawnCautioneer();
	void SpawnWanderer();
	bool spawnsComplete;
	int m_enemiesSpawned;
private:
	
	float spawnInterval, lastSpawn;
	int m_maxEnemiesToSpawn;
	Engine *m_engine;

	
};