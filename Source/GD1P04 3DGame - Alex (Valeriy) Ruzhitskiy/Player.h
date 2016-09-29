//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Player.h
//  Description: Player gameobject header file
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"
#include "GameObject.h"
#include "Bullet.h"

class Player : public GameObject
{
public:
	Player();
	Player(glm::vec2 pos, Texture2D sprite, Engine *engine);
	~Player();

	void Update(GLfloat dt);
	void Render(SpriteRenderer &renderer);
	void ProcessInput(GLboolean keys[1024], GLfloat dt);
	float maxSpeed;
private:
	float fireTimer, lastShot;
	void UpdateRotation();
	void ApplyForce(glm::vec2 _force);
	void Fire();
	void KeepInBounds();

	Engine *m_engine;
};