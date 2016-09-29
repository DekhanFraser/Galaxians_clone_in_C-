//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Player.cpp
//  Description: Player gameobject implementation
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include "Player.h"
#include "ResourceManager.h"

/*
* Function - Player()
* Default constructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Player::Player()
{
}

/*
* Function - Player(@param)
* Parametrized constructor
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 pos -player position
* Texture2D sprite - player sprite
* Engine *engine - pointer to game engine
*/
Player::Player(glm::vec2 pos, Texture2D sprite, Engine *engine)
	:GameObject(pos, glm::vec2(sprite.Width, sprite.Height), sprite), maxSpeed(300.0f), m_engine(engine), fireTimer(0.1f), lastShot(0.0f)
{
	iff = Friend;
}

/*
* Function - ~Player()
* Default destructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Player::~Player()
{
	m_engine = 0;
}

/*
* Function - Update(@param)
* Update function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Player::Update(GLfloat dt)
{
	lastShot += dt;
	Velocity *= 0.95;
	if (Velocity.x > maxSpeed)
	{
		Velocity.x = maxSpeed;
	}
	if (Velocity.x < -maxSpeed)
	{
		Velocity.x = -maxSpeed;
	}
	if (Velocity.y > maxSpeed)
	{
		Velocity.y = maxSpeed;
	}
	if (Velocity.y < -maxSpeed)
	{
		Velocity.y = -maxSpeed;
	}

	Position += Velocity * dt;
	KeepInBounds();
	UpdateRotation();
}

/*
* Function - Render(@param)
* Rendering function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* SpriteRenderer - reference to renderer
*/
void Player::Render(SpriteRenderer &renderer)
{
	GameObject::Render(renderer);
}

/*
* Function - ProcessInput(@param)
* Update input function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Player::ProcessInput(GLboolean Keys[1024], GLfloat dt)
{
	if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT])
	{
		ApplyForce(glm::vec2(-50.0f, 0.0f));
	}
	if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT])
	{
		ApplyForce(glm::vec2(50.0f, 0.0f));
	}
	if (Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP])
	{
		ApplyForce(glm::vec2(0.0f, -50.0f));
	}
	if (Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN])
	{
		ApplyForce(glm::vec2(0.0f, 50.0f));
	}
	if (Keys[GLFW_KEY_SPACE])
	{
		Fire();
	}
	//Update player rotation
	if ((Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) && (Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP]))
	{
		dir = AW;
	}
	else if ((Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP]) && (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]))
	{
		dir = WD;
	}
	else if ((Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) && (Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN]))
	{
		dir = DS;
	}
	else if ((Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN]) && (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]))
	{
		dir = SA;
	}
	else if ((Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]))
	{
		dir = A;
	}
	else if ((Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP]))
	{
		dir = W;
	}
	else if ((Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]))
	{
		dir = D;
	}
	else if ((Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN]))
	{
		dir = S;
	}
}

/*
* Function - UpdateRotation()
* Update object sprite rotation
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Player::UpdateRotation()
{
	switch (dir)
	{
	case A:
		Rotation = glm::radians(-90.0f);
		break;
	case AW:
		Rotation = glm::radians(-45.0f);
		break;
	case W:
		Rotation = glm::radians(0.0f);
		break;
	case WD:
		Rotation = glm::radians(45.0f);
		break;
	case D:
		Rotation = glm::radians(90.0f);
		break;
	case DS:
		Rotation = glm::radians(135.0f);
		break;
	case S:
		Rotation = glm::radians(180.0f);
		break;
	case SA:
		Rotation = glm::radians(-135.0f);
		break;
	default:
		break;
	}
}

/*
* Function - ApplyForce(@param)
* Apply steering force to object
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 force - force to apply
*/
void Player::ApplyForce(glm::vec2 _force)
{
	Velocity += _force;
}

/*
* Function - Fire()
* Fire weapon
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Player::Fire()
{
	if (lastShot > fireTimer)
	{
		Bullet bullet(Position, glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("bullet1"));
		switch (dir)
		{
		case A:
			bullet.Position.x += -15.0f;
			bullet.Position.y += 25.0f;
			bullet.Velocity = glm::vec2(-1000.0f, 0.0f);
			break;
		case AW:
			bullet.Position.x += -5.0f;
			bullet.Position.y += -5.0f;
			bullet.Velocity = glm::vec2(-1000.0f, -1000.0f);
			break;
		case W:
			bullet.Position.x += 25.0f;
			bullet.Position.y += -10.0f;
			bullet.Velocity = glm::vec2(0.0f, -1000.0f);
			break;
		case WD:
			bullet.Position.x += Size.x - 5.0f;
			bullet.Position.y += - 5.0f;
			bullet.Velocity = glm::vec2(1000.0f, -1000.0f);
			break;
		case D:
			bullet.Position.x += Size.x;
			bullet.Position.y += 25.0f;
			bullet.Velocity = glm::vec2(1000.0f, 0.0f);
			break;
		case DS:
			bullet.Position.x += Size.x -10.0f;
			bullet.Position.y += Size.y -10.0f;
			bullet.Velocity = glm::vec2(1000.0f, 1000.0f);
			break;
		case S:
			bullet.Position.x += 25.0f;
			bullet.Position.y += Size.y;
			bullet.Velocity = glm::vec2(0.0f, 1000.0f);
			break;
		case SA:
			bullet.Position.x += -5.0f;
			bullet.Position.y += Size.y -10.0f;
			bullet.Velocity = glm::vec2(-1000.0f, 1000.0f);
			break;
		default:
			break;
		}
		bullet.iff = Friend;
		m_engine->Bullets.push_back(bullet);
		m_engine->PlaySound("assets/audio/laser_shoot.wav", GL_FALSE);
		lastShot = 0;
	}
}

/*
* Function - KeepInBounds()
* Restrict player position to onscreen
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Player::KeepInBounds()
{
	if (Position.y < 0.0f)
	{
		Position.y = 0.0f;
	}
	if (Position.y > m_engine->Height - Size.y)
	{
		Position.y = m_engine->Height - Size.y;
	}
	if (Position.x < 0.0f)
	{
		Position.x = 0.0f;
	}
	if (Position.x > m_engine->Width - Size.x)
	{
		Position.x = m_engine->Width - Size.x;
	}
}