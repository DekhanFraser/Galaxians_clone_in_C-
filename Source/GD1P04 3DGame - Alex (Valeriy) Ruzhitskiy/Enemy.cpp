//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Enemy.cpp
//  Description: Enemy gameobject implementation file
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include "Enemy.h"
#include "Engine.h"

/*
* Function - Enemy()
* Default constructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Enemy::Enemy()
{
}

/*
* Function - Enemy(@param)
* Default constructor
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 pos - object position
* Behavior behavior - enemy behavior patern
* glm::vec2 velocity - object velocity at start
* Texture2D sprite - object sprite
* Engine *engine - pointer to engine object
*/
Enemy::Enemy(glm::vec2 pos, Behavior behavior, glm::vec2 velocity, Texture2D sprite, Engine *engine)
	: GameObject(pos, glm::vec2(60, 60), sprite, glm::vec3(1.0f), velocity), m_behavior(behavior), maxSpeed(400.0f), m_engine(engine),
	behaviorTimer(0.0f), behaviorReset(5.0f), behaviorChanged(false), fireInterval(3.0f), fireTimer(0.0f)
{
}

/*
* Function - ~Enemy()
* Default destructor
* Author - Alex (Valeriy) Ruzhitskiy
*/
Enemy::~Enemy()
{
}

/*
* Function - Update(@param)
* Update function
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* GLfloat dt - deltatime per frame
*/
void Enemy::Update(GLfloat dt)
{
	DoAiWork();
	fireTimer += dt;
	behaviorTimer += dt;
	Position += Velocity * dt;
	UpdateRotation();
}

/*
* Function - DoAIWork()
* Process AI behaviors
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Enemy::DoAiWork()
{
	switch (m_behavior)
	{
	case Suicide:
		SuicideMode();
		break;
	case Cautious:
		CautiousMode();
		break;
	case Wanderer:
		WanderMode();
		break;
	default:
		break;
	}
}

/*
* Function - UpdateRotation()
* Update object sprite rotation
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Enemy::UpdateRotation()
{
	if (Velocity.x < 0 && Velocity.y < 0 && Velocity.x < Velocity.y)
	{
		Rotation = glm::radians(-90.0f);
	}
	else if (Velocity.x < 0 && Velocity.y < 0 && Velocity.x > Velocity.y)
	{
		Rotation = glm::radians(0.0f);
	}
	else if (Velocity.x > 0 && Velocity.y < 0 && -Velocity.x > Velocity.y)
	{
		Rotation = glm::radians(0.0f);
	}
	else if (Velocity.x > 0 && Velocity.y < 0 && -Velocity.x < Velocity.y)
	{
		Rotation = glm::radians(90.0f);
	}
	else if (Velocity.x > 0 && Velocity.y > 0 && Velocity.x < Velocity.y)
	{
		Rotation = glm::radians(180.0f);
	}
	else if (Velocity.x > 0 && Velocity.y > 0 && Velocity.x > Velocity.y)
	{
		Rotation = glm::radians(90.0f);
	}
	else if (Velocity.x < 0 && Velocity.y > 0 && Velocity.x > -Velocity.y)
	{
		Rotation = glm::radians(180.0f);
	}
	else if (Velocity.x < 0 && Velocity.y > 0 && Velocity.x < -Velocity.y)
	{
		Rotation = glm::radians(-90.0f);
	}
}

/*
* Function - ApplyForce(@param)
* Apply steering force to object
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 force - force to apply
*/
void Enemy::ApplyForce(glm::vec2 _force)
{
	Velocity += _force;
}

/*
* Function - LimitForce(@param)
* Limit steering force
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* glm::vec2 force - force to apply
* float maxForce - force limiter
*/
glm::vec2 Enemy::LimitForce(glm::vec2 _force, float _maxForce)
{
	float length = abs(glm::length(_force));

	if (length > (_maxForce * _maxForce))
	{
		float ratio = _maxForce / sqrt(length);
		_force *= ratio;
	}

	return _force;
}

/*
* Function - CautiousMode()
* Process Suicide behavior
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Enemy::SuicideMode()
{
	SeekMode(m_engine->GetPlayerPos());
}

/*
* Function - CautiousMode()
* Process cautious behavior
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Enemy::CautiousMode()
{
	if (!behaviorChanged)
	{
		if (glm::distance(m_engine->GetPlayerPos(), Position) < 350.0f)
		{
			int chance = rand() % 2;
			switch (chance)
			{
			case 0:
				state = Flee;
				break;
			case 1:
				state = Wander;
				break;
			default:
				break;
			}
			behaviorChanged = true;
		}
		else
		{
			state = Seek;
		}
	}
	else
	{
		state = Flee;
		if (behaviorTimer > behaviorReset)
		{
			behaviorTimer = 0.0f;
			behaviorChanged = false;
		}
	}
	if (fireTimer > fireInterval)
	{
		Fire();
	}
	switch (state)
	{
	case Seek:
		SeekMode(m_engine->GetPlayerPos());
		break;
	case Flee:
		FleeMode(m_engine->GetPlayerPos());
		break;
	case Wander:
		WanderMode();
		break;
	default:
		break;
	}
}

/*
* Function - Fire()
* Fire weapon
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Enemy::Fire()
{
	Bullet bullet(Position, glm::vec2(0.0f, 0.0f), ResourceManager::GetTexture("bullet2"));
	bullet.Velocity = glm::normalize(m_engine->GetPlayerPos() - Position) * 250.0f;
	bullet.iff = Hostile;
	m_engine->Bullets.push_back(bullet);
	m_engine->PlaySound("assets/audio/laser_shoot.wav", GL_FALSE);
	fireTimer = 0;
}


/*
* Function - SeekMode
* AI function to seek target
* @param :
* vec2 target position
* Author - Alex (Valeriy) Ruzhitskiy
*/
void Enemy::SeekMode(glm::vec2 _target)
{
	glm::vec2 desired = _target - Position;
	desired = glm::normalize(desired);
	desired *= maxSpeed;

	glm::vec2 steer = desired - Velocity;

	steer = LimitForce(steer, 0.2);
	ApplyForce(steer);
}

/*
* Function - PursueMode(@param)
* AI function to pursue target
* @param :
* vec2 target position
* Author - Alex (Valeriy) Ruzhitskiy and Daniel Pringle
*/
void Enemy::FleeMode(glm::vec2 _target)
{
	glm::vec2 negative = glm::vec2(-1.0f, -1.0f);

	glm::vec2 desired = Position - _target;
	desired = glm::normalize(desired);
	desired *= maxSpeed;

	glm::vec2 steer = desired - Velocity;
	steer = LimitForce(steer, 0.2f);

	ApplyForce(steer);
}

/*
* Function - WanderMode()
* AI function to wander to random target
* Author - Alex (Valeriy) Ruzhitskiy and Daniel Pringle
*/
void Enemy::WanderMode()
{
	int ChanceToWander = rand() % 1000;

	int NewX = rand() % (int)m_engine->Width * 2 - (int)m_engine->Width;
	int NewY = rand() % (int)m_engine->Height * 2 - (int)m_engine->Height;

	if (ChanceToWander > 990)
	{
		Fire();
		target.x = (float)NewX;
		target.y = (float)NewY;
	}
	else if (target.x - Position.x <= 1.0f && target.y - Position.y <= 1.0f)
	{
		Fire();
		target.x = (float)NewX;
		target.y = (float)NewY;
	}

	SeekMode(target);
}