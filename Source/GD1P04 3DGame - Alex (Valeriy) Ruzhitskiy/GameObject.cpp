//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : GameObject.cpp
//  Description: GameObject implementation
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include "GameObject.h"

/*
* Function - Gameobject()
* Default constructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
GameObject::GameObject()
	: Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

/*
* Function - GameObject(@param)
* parametrized constructor
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 pos - object poistion
* glm::vec2 size - sprite size (for collisions)
* Texture2D sprite - sprite texture
* glm::vec3 color - sprite color
* glm::vec2 velocity - object starting velocity
*/
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) { }

/*
* Function - Update(@param)
* Update function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void GameObject::Update(GLfloat dt)
{

}

/*
* Function - Render(@param)
* Rendering function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* SpriteRenderer - reference to renderer
*/
void GameObject::Render(SpriteRenderer &renderer)
{
	if (!Destroyed)
	{
		renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	}
}