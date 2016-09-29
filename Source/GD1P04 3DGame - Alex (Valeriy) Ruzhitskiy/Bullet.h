//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Bullet.h
//  Description: Bullet object header
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	Bullet(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	~Bullet();
	void Update(GLfloat dt);
	bool outOfBounds;
private:
	float lifetime, maxlifetime;
};