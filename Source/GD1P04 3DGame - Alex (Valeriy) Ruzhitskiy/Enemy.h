//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Enemy.h
//  Description: Enemy gameobject header file
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Texture2D.h"

class Engine;

enum Behavior
{
	Suicide,
	Cautious,
	Wanderer
};

enum STATE
{
	Seek,
	Flee,
	Wander
};

class Enemy : public GameObject
{
public:
	Enemy();
	Enemy(glm::vec2 pos, Behavior behavior, glm::vec2 velocity, Texture2D sprite, Engine *engine);
	~Enemy();

	void Update(GLfloat dt);
private:
	Behavior m_behavior;
	void DoAiWork();
	void UpdateRotation();
	void ApplyForce(glm::vec2 _force);
	glm::vec2 LimitForce(glm::vec2 _force, float _maxForce);
	void SuicideMode();
	void CautiousMode();
	void SeekMode(glm::vec2 _target);
	void FleeMode(glm::vec2 _target);

	void WanderMode();

	void Fire();

	Engine *m_engine;
	STATE state;
	glm::vec2 target;
	float maxSpeed;
	float behaviorTimer, behaviorReset;
	float fireTimer, fireInterval;
	bool behaviorChanged;
};