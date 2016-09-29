//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Engine.cpp
//  Description: Main game engine implamentation
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include <algorithm>
#include <sstream>

#include <irrrKlang\irrKlang.h>
using namespace irrklang;

#include "Engine.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "TextRenderer.h"
#include "Player.h"
#include "EnemySpawner.h"

// Engine state variables
SpriteRenderer    *Renderer;
Player			  *Player1;
ParticleGenerator *Particles;
PostProcessor     *Effects;
ISoundEngine      *SoundEngine = createIrrKlangDevice();
GLfloat            ShakeTime = 0.0f;
TextRenderer      *Text;
int				  Score;

/*
* Function - Engine(@param)
* Default constructor
* @param :
* GLuint width - screen width
* Gluint height - screen height
* Author - Alex (Valeriy) Ruzhitskiy
*/
Engine::Engine(GLuint width, GLuint height)
	: State(GAME_MENU), Width(width), Height(height), Keys(), Level(0), Lives(3)
{

}

/*
* Function - ~Engine()
* Default destructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Engine::~Engine()
{
	delete Player1;
	delete Renderer;
	delete Particles;
	delete Effects;
	delete Text;
	Enemies.clear();
	Bullets.clear();
	SoundEngine->drop();
}

/*
* Function - Init()
* Engine init script
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::Init()
{
	// Load shaders
	ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("assets/shaders/particle.vert", "assets/shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("assets/shaders/postprocessor.vert", "assets/shaders/postprocessor.frag", nullptr, "postprocessing");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("assets/textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("assets/textures/enemyTurret.png", GL_TRUE, "enemyTurret");
	ResourceManager::LoadTexture("assets/textures/enemySuicider.png", GL_TRUE, "enemySuicider");
	ResourceManager::LoadTexture("assets/textures/enemyCautioneer.png", GL_TRUE, "enemyCautioneer");
	ResourceManager::LoadTexture("assets/textures/player.png", GL_TRUE, "player");
	ResourceManager::LoadTexture("assets/textures/bullet1.png", GL_TRUE, "bullet1");
	ResourceManager::LoadTexture("assets/textures/bullet2.png", GL_TRUE, "bullet2");
	ResourceManager::LoadTexture("assets/textures/particle.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("assets/textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("assets/textures/powerup_extra_life.png", GL_TRUE, "powerup_extra_life");
	ResourceManager::LoadTexture("assets/textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("assets/textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("assets/fonts/OCRAEXT.TTF", 24);
	
	// Configure game objects
	Player1 = new Player(glm::vec2(Width / 2, Height / 2), ResourceManager::GetTexture("player"), this);
	
	// Audio
	SoundEngine->play2D("assets/audio/galaxians.mp3", GL_TRUE);

	// Set base level stats to let game play in the background of main menu
	Score = 0;
	Level = 1;
	ResetLevel(Level);
	ResetPlayer();
}

/*
* Function - Update(@param)
* Update function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Engine::Update(GLfloat dt)
{
	// Update objects
	for (EnemySpawner &spawner : Spawners)
	{
		spawner.Update(dt);
	}
	for (Enemy &enemy : Enemies)
	{
		enemy.Update(dt);
	}
	for (Bullet &bullet : Bullets)
	{
		bullet.Update(dt);
	}
	// Update player
	Player1->Update(dt);
	// Check for collisions
	this->DoCollisions();
	// Update particles
	Particles->Update(dt, *Player1, 2, glm::vec2(32.0f, 32.0f));
	// Update PowerUps
	this->UpdatePowerUps(dt);
	// Reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}
	// Check win condition
	if (this->State == GAME_ACTIVE && Spawners.size() > 0)
	{
		bool levelComplete = true;
		for (EnemySpawner &spawner : Spawners)
		{
			if (!spawner.spawnsComplete)
			{
				levelComplete = false;
			}
		}
		if (levelComplete && Enemies.size() == 0)
		{
			Level++;
			ResetLevel(Level);
		}
		if (Lives <= 0)
		{
			State = GAME_LOSE;
			Lives = 0;
			Effects->Chaos = true;
			CleanLevel();
		}
	}
	CleanupDestroyedObjects();
}

/*
* Function - ProcessInput(@param)
* Update input function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Engine::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			CleanLevel();
			ResetLevel(1);
			ResetPlayer();
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
	}
	if (this->State == GAME_LOSE)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			Effects->Chaos = GL_FALSE;
			ResetLevel(1);
			ResetPlayer();
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
	}
	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Effects->Chaos = GL_FALSE;
			this->State = GAME_MENU;
		}
	}
	if (this->State == GAME_ACTIVE)
	{
		Player1->ProcessInput(Keys, dt);
	}
}

/*
* Function - Render()
* Render all objects depending on current egine state
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::Render()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN || this->State == GAME_LOSE)
	{
		// Begin rendering to postprocessing quad
		Effects->BeginRender();
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		// Draw level
		// Draw PowerUps
		for (PowerUp &powerUp : this->PowerUps)
			if (!powerUp.Destroyed)
				powerUp.Render(*Renderer);
		for (Enemy enemy : this->Enemies)
		{
			enemy.Render(*Renderer);
		}
		for (Bullet bullet : this->Bullets)
		{
			bullet.Render(*Renderer);
		}
		// Draw particles
		Particles->Render();
		// Draw player
		Player1->Render(*Renderer);
		// End rendering to postprocessing quad
		Effects->EndRender();
		// Render postprocessing quad
		Effects->Render(glfwGetTime());
	}
	if (this->State == GAME_ACTIVE)
	{
		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
		ss.str("");
		ss.clear();
		ss << Score;
		Text->RenderText("Score: " + ss.str(), 5.0f, 30.0f, 1.0f);
		ss.str("");
		ss.clear();
		ss << Level;
		Text->RenderText("Level: " + ss.str(), 600.0f, 5.0f, 1.0f);

		//DisplayeDebug();
	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 500.0f, this->Height / 2 - 100, 1.0f);
	}
	if (this->State == GAME_LOSE)
	{
		Text->RenderText("You LOST!!!", 550.0f, this->Height / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry or ESC to quit", 400.0f, this->Height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText("You WON!!!", 550.0f, this->Height / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry or ESC to quit", 400.0f, this->Height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
}

/*
* Function - CleanLevel()
* Clean level upon lose or win
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::CleanLevel()
{
	Spawners.clear();
	Enemies.clear();
	Bullets.clear();
	PowerUps.clear();
}

/*
* Function - DisplayDebug()
* Display debug information
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::DisplayeDebug()
{
	// Render text (don't include in postprocessing)
	std::stringstream ss; ss << this->Lives;
	ss << Player1->Position.x << ";" << Player1->Position.y;
	Text->RenderText("Position:" + ss.str(), 900.0f, 30.0f, 1.0f);
	ss.str("");
	ss.clear();
	ss << Bullets.size();
	Text->RenderText("Bullets:" + ss.str(), 900.0f, 55.0f, 1.0f);
	ss.str("");
	ss.clear();
	ss << Spawners.size();
	Text->RenderText("Spawners:" + ss.str(), 900.0f, 80.0f, 1.0f);
	ss.str("");
	ss.clear();
	ss << Enemies.size();
	Text->RenderText("Enemies:" + ss.str(), 900.0f, 105.0f, 1.0f);
	ss.str("");
	ss.clear();
	int totalEnemiesSpawned = 0;
	for (EnemySpawner &spawner : Spawners)
	{
		totalEnemiesSpawned += spawner.m_enemiesSpawned;
	}
	ss << totalEnemiesSpawned;
	Text->RenderText("Enemies Spawned: " + ss.str(), 900.0f, 130.0f, 1.0f);
	ss.str("");
	ss.clear();
	ss << Player1->Velocity.x << ";" << Player1->Velocity.y;
	Text->RenderText("Velocity:" + ss.str(), 900.0f, 155.0f, 1.0f);
	bool levelComplete = true;
	for (EnemySpawner &spawner : Spawners)
	{
		if (!spawner.spawnsComplete)
		{
			levelComplete = false;
		}
	}
	if (levelComplete)
	{
		Text->RenderText("Spawn complete" + ss.str(), 900.0f, 130.0f, 1.0f);
	}
}

/*
* Function - PlaySound(@param)
* Helper sound function, can be used to play sounds from any function which has pointer to engine
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* const char *path - path to sound file
* GLboolean loop - should the file be looped
*/
void Engine::PlaySound(const char *path, GLboolean loop)
{
	SoundEngine->play2D(path, loop);
}

/*
* Function - GetPlayerPos()
* Helper function for AI to get player position for targeting
* Author - Alex (Valeriy) Ruzhitskiy
*/
glm::vec2 Engine::GetPlayerPos()
{
	return Player1->Position;
}

/*
* Function - LoadLevel(@param)
* Loads specified difficulty level
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* int level - desired difficulty level
*/
void Engine::LoadLevel(int level)
{
	Level = level;
	AddSpawner(glm::vec2(0.0f, 0.0f));
	AddSpawner(glm::vec2(Width, 0.0f));
	AddSpawner(glm::vec2(Width, Height));
	AddSpawner(glm::vec2(0.0f, Height));
}

/*
* Function - AddSpawner(@param)
* Helper function to add enemy spawners
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 pos - spawner position
*/
void Engine::AddSpawner(glm::vec2 pos)
{
	EnemySpawner Spawner(this);
	Spawner.Position = pos;
	Spawner.Init(Level);
	Spawners.push_back(Spawner);
}

/*
* Function - CleanupDestroyedObjects()
* Helper function to remove destroyed objects from object arrays
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::CleanupDestroyedObjects()
{
	if (Bullets.size() > 0)
	{
		for (unsigned int i = 0; i < Bullets.size(); i++)
		{
			if (Bullets[i].Destroyed)
			{
				Bullets.erase(Bullets.begin() + i);
				i--;
			}
		}
	}
	if (Enemies.size() > 0)
	{
		for (unsigned int i = 0; i < Enemies.size(); i++)
		{
			if (Enemies[i].Destroyed)
			{
				Enemies.erase(Enemies.begin() + i);
				i--;
			}
		}
	}
}

/*
* Function - ResetLevel(@param)
* Helper function to reset the difficulty level
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* int level - desired difficulty level
*/
void Engine::ResetLevel(int level)
{

	Level = level;
	Enemies.clear();
	Spawners.clear();
	PowerUps.clear();
	LoadLevel(Level);
	
	if (level == 1)
	{
		Score = 0;
		Lives = 3;
	}
}

/*
* Function - ResetPlayer()
* Helper function to reset player position and state
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::ResetPlayer()
{
	// Reset player stats
	Player1->Position = glm::vec2(this->Width / 2 - Player1->Size.x / 2, this->Height / 2 - Player1->Size.y / 2);
	Player1->maxSpeed = (300.0f);
	// Also disable all active powerups
	Effects->Chaos = Effects->Confuse = GL_FALSE;
}


// PowerUps
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

/*
* Function - UpdatepowerUps(@param)
* Update function for powerups
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Engine::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp &powerUp : this->PowerUps)
	{
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				// Remove powerup from list (will later be removed)
				powerUp.Activated = GL_FALSE;
				// Deactivate effects
				if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{	// Only reset if no other PowerUp of type confuse is active
						Effects->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{	// Only reset if no other PowerUp of type chaos is active
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	// Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
	// Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}

/*
* Function - ShouldSpawn(@param)
* Helper function to check if a powerup should spawn
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLint chance - desired probability
* @return :
* GLboolean - success or failed
*/
GLboolean ShouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}

/*
* Function - SpawnPowerups(@param)
* Helper function to decide wether to pawn a powerup and then spawn it if yes
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GameObject - where to spawn powerup if successful
*/
void Engine::SpawnPowerUps(GameObject &enemy)
{
	if (ShouldSpawn(75)) // 1 in 75 chance
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, enemy.Position, ResourceManager::GetTexture("powerup_speed")));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("extra-life", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, enemy.Position, ResourceManager::GetTexture("powerup_extra_life")));
	if (ShouldSpawn(15)) // Negative powerups should spawn more often
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, enemy.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (ShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, enemy.Position, ResourceManager::GetTexture("powerup_chaos")));
}

/*
* Function - ActivatePowerup(@param)
* Helper function to activate a powerup upon pickup
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* Powerup gameobject - which powerup got picked up to activate
*/
void Engine::ActivatePowerUp(PowerUp &powerUp)
{
	// Initiate a powerup based type of powerup
	if (powerUp.Type == "speed")
	{
		Player1->maxSpeed += 50;
	}
	else if (powerUp.Type == "extra-life")
	{
		Lives++;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = GL_TRUE; // Only activate if chaos wasn't already active
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = GL_TRUE;
	}
}

/*
* Function - IsOtherPowerUpActive(@param)
* Helper function to check powerup activity
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* vector PowerUps - vector of the powerups
* string type - type of powerup
*/
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	// Check if another PowerUp of the same type is still active
	// in which case we don't disable its effect (yet)
	for (const PowerUp &powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}

// Collision detection
GLboolean CheckCollision(GameObject &one, GameObject &two);

/*
* Function - DoCollisions()
* Update function for collisions
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Engine::DoCollisions()
{
	for (Enemy &enemy : Enemies)
	{
		if (CheckCollision(*Player1, enemy))
		{
			Lives--;
			ResetPlayer();
			enemy.Destroyed = true;
			Score += 100;
			Effects->Shake = true;
			ShakeTime = 0.2f;
		}
	}
	for (Bullet &bullet : Bullets)
	{
		if (bullet.iff == Friend)
		{
			for (Enemy &enemy : Enemies)
			{
				if (CheckCollision(bullet, enemy))
				{
					bullet.Destroyed = true;
					enemy.Destroyed = true;
					SpawnPowerUps(enemy);
					Score += 100;
					Effects->Shake = true;
					ShakeTime = 0.1f;
				}
			}
		}
		if (bullet.iff == Hostile)
		{
			if (CheckCollision(*Player1, bullet))
			{
				Lives--;
				ResetPlayer();
				bullet.Destroyed = true;
				Effects->Shake = true;
				ShakeTime = 0.2f;
			}
		}
	}

	// Also check collisions on PowerUps and if so, activate them
	for (PowerUp &powerUp : this->PowerUps)
	{
		if (!powerUp.Destroyed)
		{
			// First check if powerup passed bottom edge, if so: keep as inactive and destroy
			if (powerUp.Position.y >= this->Height)
				powerUp.Destroyed = GL_TRUE;

			if (CheckCollision(*Player1, powerUp))
			{	// Collided with player, now activate powerup
				ActivatePowerUp(powerUp);
				powerUp.Destroyed = GL_TRUE;
				powerUp.Activated = GL_TRUE;
				SoundEngine->play2D("assets/audio/powerup.wav", GL_FALSE);
			}
		}
	}
}

/*
* Function - CheckCollision(@param)
* Actual AABB collision detection function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GameObject one,
* GameObject two
*/
GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// Collision x-axis?
	GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}