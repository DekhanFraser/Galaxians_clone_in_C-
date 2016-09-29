//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Engine.h
//  Description: Main game engine header file.
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "EnemySpawner.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PowerUp.h"

class GameObject;

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_LOSE,
	GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Engine
{
public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	GLboolean              KeysProcessed[1024];
	GLuint                 Width, Height;
	std::vector<Bullet>    Bullets;
	std::vector<Enemy>     Enemies;
	std::vector<EnemySpawner> Spawners;
	std::vector<PowerUp>   PowerUps;
	GLuint                 Level;
	int					   Lives;
	// Constructor/Destructor
	Engine(GLuint width, GLuint height);
	~Engine();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void CleanLevel();
	void DisplayeDebug();
	void PlaySound(const char *path, GLboolean loop);
	glm::vec2 GetPlayerPos();
	void LoadLevel(int level);
	void AddSpawner(glm::vec2 pos);
	void DoCollisions();
	// Reset
	void CleanupDestroyedObjects();
	void ResetLevel(int level);
	void ResetPlayer();
	// Powerups
	void SpawnPowerUps(GameObject &block);
	void ActivatePowerUp(PowerUp & powerUp);
	void UpdatePowerUps(GLfloat dt);
};