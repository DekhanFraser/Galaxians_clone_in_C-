//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : EnemySpwaner.cpp
//  Description: EnemySpawner gameobject implementation.
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include "EnemySpawner.h"
#include "Engine.h"

/*
* Function - EnemySpawner(@param)
* Default constructor
* @param :
* Engine *engine - pointer to main game engine
* Author - Alex (Valeriy) Ruzhitskiy
*/
EnemySpawner::EnemySpawner(Engine *engine)
	: m_engine(engine), m_enemiesSpawned(0), lastSpawn(0.0f)
{
}

/*
* Function - ~EnemySpawner()
* Default destructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
EnemySpawner::~EnemySpawner()
{
	m_engine = nullptr;
}

/*
* Function - Init(@param)
* Initializer for the spawner object
* @param :
* int level - difficulty level to init
* Author - Alex (Valeriy) Ruzhitskiy
*/
void EnemySpawner::Init(int level)
{
	spawnsComplete = false;
	lastSpawn = 0.0f;
	m_enemiesSpawned = 0;
	m_maxEnemiesToSpawn = level * 2;
	spawnInterval = 5.0f / level;
	if (spawnInterval < 1.0f)
	{
		spawnInterval = 1.0f;
	}
}

/*
* Function - Update(@param)
* Update function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void EnemySpawner::Update(GLfloat dt)
{
	lastSpawn += dt;
	if (lastSpawn > spawnInterval && m_enemiesSpawned < m_maxEnemiesToSpawn)
	{
		if (m_engine->State == GAME_MENU)
		{
			Behavior _b = static_cast<Behavior>((rand() % 2) + 1);
			SpawnEnemy(_b);
		}
		else
		{
			Behavior _b = static_cast<Behavior>(rand() % 3);
			SpawnEnemy(_b);
		}
	}
	if (m_enemiesSpawned == m_maxEnemiesToSpawn)
	{
		spawnsComplete = true;
	}
}

/*
* Function - SpawnEnemy(@param)
* Spawns enemy with set behavior patern
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* Behavior - behavior patern
*/
void EnemySpawner::SpawnEnemy(Behavior behavior)
{
	switch (behavior)
	{
	case Suicide:
		SpawnSuicider();
		break;
	case Cautious:
		SpawnCautioneer();
		break;
	case Wanderer:
		SpawnWanderer();
		break;
	default:
		break;
	}
	
}

/*
* Function - SpawnSuicider()
* Spawn suicider enemy
* Author - Alex (Valeriy) Ruzhitskiy
*/
void EnemySpawner::SpawnSuicider()
{
	Enemy enemy(Position, Suicide, glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("enemySuicider"), m_engine);
	m_engine->Enemies.push_back(enemy);
	m_enemiesSpawned++;
	lastSpawn = 0;
}

/*
* Function - SpawnCautioneer()
* Spawn cautioneer enemy
* Author - Alex (Valeriy) Ruzhitskiy
*/
void EnemySpawner::SpawnCautioneer()
{
	Enemy enemy(Position, Cautious, glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("enemyCautioneer"), m_engine);
	m_engine->Enemies.push_back(enemy);
	m_enemiesSpawned++;
	lastSpawn = 0;
}

/*
* Function - SpawnWanderer()
* Spawn wanderer enemy
* Author - Alex (Valeriy) Ruzhitskiy
*/
void EnemySpawner::SpawnWanderer()
{
	Enemy enemy(Position, Wanderer, glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("enemyTurret"), m_engine);
	m_engine->Enemies.push_back(enemy);
	m_enemiesSpawned++;
	lastSpawn = 0;
}