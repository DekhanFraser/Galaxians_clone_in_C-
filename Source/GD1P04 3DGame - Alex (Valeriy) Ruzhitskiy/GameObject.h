//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : GameObject.h
//  Description: GameObject header file
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.

enum PlayerDir
{
	A, AW, W, WD,
	D, DS, S, SA
};

enum IFF
{
	Friend,
	Hostile,
	Neutral
};

class GameObject
{
public:
	// Object state
	glm::vec2   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// Render state
	Texture2D   Sprite;
	// Constructor(s)
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	// Draw sprite
	virtual void Update(GLfloat dt);
	virtual void Render(SpriteRenderer &renderer);
	PlayerDir dir;
	IFF iff;
};