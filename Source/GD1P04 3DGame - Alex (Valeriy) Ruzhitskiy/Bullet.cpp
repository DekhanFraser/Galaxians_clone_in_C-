//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Bullet.cpp
//  Description: Bullet object implementation file. Inherits from GameObject
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include "Bullet.h"

/*
* Function - Bullet()
* Default constructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Bullet::Bullet()
{
}

/*
* Function - Bullet(@param)
* Parametrized contructor
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 position - object position
* glm::vec2 size - sprite size
* Texture2D - object sprite
*/
Bullet::Bullet(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
	:GameObject(pos, glm::vec2(sprite.Width, sprite.Height), sprite), outOfBounds(false), maxlifetime(4.0f), lifetime(0.0f)
{

}

/*
* Function - ~Bullet()
* Default destructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Bullet::~Bullet()
{
}

/*
* Function - Update(@param)
* Update function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Bullet::Update(GLfloat dt)
{
	lifetime += dt;
	Position += Velocity * dt;
	if (lifetime > maxlifetime)
	{
		Destroyed = true;
	}
}