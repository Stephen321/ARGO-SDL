#include "CollisionSystem.h"

#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "GunComponent.h"
#include "AIComponent.h"
#include "DestructionComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"
#include "StatusEffectComponent.h"

#include "ConstHolder.h"
#include "Helpers.h"

#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>



CollisionSystem::CollisionSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, float updateRate)
	: System(updateRate)
	, _interactionSystemEvents(events)
{
}

CollisionSystem::~CollisionSystem()
{
}


void CollisionSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

				collider->body->SetActive(collider->setActive);
			}
		}
	}
}

void CollisionSystem::BeginContact(b2Contact* contact)
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	if (bodyAUserData != "Obstacle" && bodyBUserData != "Obstacle")
	{
		Entity* player = nullptr;
		Entity* other = nullptr;

		FindPlayer(contact, player, other);

		if (player != nullptr && other != nullptr)
		{
			if (other->GetType() == EntityType::AI || other->GetType() == EntityType::Player)
			{
				CheckCharacterToCharacterCollision(player, other);
				std::cout << "CHARACTER->CHARACTER: " << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
			}
			else
			{
				CheckCharacterToObjectCollision(player, other);
				std::cout << "CHARACTER->OBJECT: " << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
			}
		}
	}
	/*else
	{
		if (bodyAUserData != "Obstacle")
		{
			Entity* flag = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

			if (flag->GetType() == EntityType::Flag)
			{
				PhysicsComponent* physics = static_cast<PhysicsComponent*>(flag->GetComponent(Component::Type::Physics));

				physics->xVelocity = -physics->xVelocity;
				physics->yVelocity = -physics->yVelocity;

				std::cout << "FLAG->Obstacle" << std::endl;
			}
		}
		else
		{
			Entity* flag = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());

			if (flag->GetType() == EntityType::Flag)
			{
				PhysicsComponent* physics = static_cast<PhysicsComponent*>(flag->GetComponent(Component::Type::Physics));

				physics->xVelocity = -physics->xVelocity;
				physics->yVelocity = -physics->yVelocity;

				std::cout << "FLAG->Obstacle" << std::endl;
			}
		}
	}*/
}

void CollisionSystem::CheckCharacterToObjectCollision(Entity*& player, Entity*& other)
{
	switch (other->GetType())
	{
	case EntityType::Checkpoint:

		break;

	case EntityType::PowerUp:

		break;

	case EntityType::Bullet:

		break;

	case EntityType::Flag:
		if (!static_cast<StatusEffectComponent*>(player->GetComponent(Component::Type::StatusEffect))->staggered)
		{
			static_cast<FlagComponent*>(player->GetComponent(Component::Type::Flag))->hasFlag = true;
			static_cast<ColliderComponent*>(other->GetComponent(Component::Type::Collider))->setActive = false;

			_interactionSystemEvents.at(InteractionSystemEvent::FlagPicked).push_back(std::pair<Entity*, Entity*>(player, other));
		}

		break;
	}
}
void CollisionSystem::CheckCharacterToCharacterCollision(Entity*& player, Entity*& other)
{
	FlagComponent* pFlag = static_cast<FlagComponent*>(player->GetComponent(Component::Type::Flag));
	FlagComponent* oFlag = static_cast<FlagComponent*>(other->GetComponent(Component::Type::Flag)); 

	StatusEffectComponent* playerStatusEffects = static_cast<StatusEffectComponent*>(player->GetComponent(Component::Type::StatusEffect));
	StatusEffectComponent* otherStatusEffects = static_cast<StatusEffectComponent*>(other->GetComponent(Component::Type::StatusEffect));

	if (pFlag->hasFlag)
	{
		if (!otherStatusEffects->staggered)
		{
			playerStatusEffects->staggered = true;
			playerStatusEffects->staggeredTimer = STAGGER_MAX_TIMER;

			_interactionSystemEvents.at(InteractionSystemEvent::FlagDropped).push_back(std::pair<Entity*, Entity*>(player, other));
		}
	}
	else if (oFlag->hasFlag)
	{
		if (!playerStatusEffects->staggered)
		{
			otherStatusEffects->staggered = true;
			otherStatusEffects->staggeredTimer = STAGGER_MAX_TIMER;

			_interactionSystemEvents.at(InteractionSystemEvent::FlagDropped).push_back(std::pair<Entity*, Entity*>(other, player));
		}
	}
}

void CollisionSystem::EndContact(b2Contact* contact)
{
	/*void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	if (bodyAUserData != "Obstacle" && bodyBUserData != "Obstacle")
	{
		Entity* player = nullptr;
		Entity* other = nullptr;

		FindPlayer(contact, player, other);

		if (player != nullptr && other != nullptr)
		{
			std::cout << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
		}
	}*/
}



void CollisionSystem::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	if (player != nullptr && other != nullptr)
	{
		EntityBounce(worldManifold, player, other);
		
	}
	else if (player != nullptr)
	{
		CharacterObstacleBounce(worldManifold, player);
	}
}

void CollisionSystem::EntityBounce(b2WorldManifold& worldManifold, Entity*& e1, Entity*& e2)
{
	PhysicsComponent* e1Physics = static_cast<PhysicsComponent*>(e1->GetComponent(Component::Type::Physics));
	PhysicsComponent* e2Physics = static_cast<PhysicsComponent*>(e2->GetComponent(Component::Type::Physics));


	float e1speed = sqrt(e1Physics->xVelocity * e1Physics->xVelocity + e1Physics->yVelocity * e1Physics->yVelocity);
	float e2speed = sqrt(e2Physics->xVelocity * e2Physics->xVelocity + e2Physics->yVelocity * e2Physics->yVelocity);

	if (e1speed != 0 && e2speed != 0)
	{
		float xVelocityTemp = e1Physics->xVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;
		float yVelocityTemp = e1Physics->yVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;

		e1Physics->xVelocity = e2Physics->xVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;
		e1Physics->yVelocity = e2Physics->yVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;

		e2Physics->xVelocity = xVelocityTemp;
		e2Physics->yVelocity = yVelocityTemp;
	}
	else
	{
		if (e1speed != 0)
		{
			e2Physics->xVelocity = e1Physics->xVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;
			e2Physics->yVelocity = e1Physics->yVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;

			e1Physics->xVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
			e1Physics->yVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
		}
		else if (e2speed != 0)
		{
			e1Physics->xVelocity = e2Physics->xVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;
			e1Physics->yVelocity = e2Physics->yVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;

			e2Physics->xVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
			e2Physics->yVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
		}
	}

	static_cast<ColliderComponent*>(e1->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(e1Physics->xVelocity, e1Physics->yVelocity));
	static_cast<ColliderComponent*>(e2->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(e2Physics->xVelocity, e2Physics->yVelocity));
}

void CollisionSystem::CharacterObstacleBounce(b2WorldManifold& worldManifold, Entity*& player)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));

	float speed = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);

	if (speed != 0)
	{
		float xDirection = physics->xVelocity / speed;
		float yDirection = physics->yVelocity / speed;

		float xDirectionNew = worldManifold.normal.x + xDirection;
		float yDirectionNew = worldManifold.normal.y + yDirection;

		float directionlenght = sqrt(xDirectionNew * xDirectionNew + yDirectionNew * yDirectionNew);

		if (directionlenght != 0)
		{
			speed *= PLAYER_WALL_RESTITUTION;
			physics->xVelocity = (xDirectionNew / directionlenght) * speed;
			physics->yVelocity = (yDirectionNew / directionlenght) * speed;
		}
		else
		{
			physics->xVelocity = -physics->xVelocity * PLAYER_WALL_RESTITUTION;
			physics->yVelocity = -physics->yVelocity * PLAYER_WALL_RESTITUTION;
		}
	}
}

void CollisionSystem::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
	/*Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);
	if (player != nullptr && other != nullptr)
		std::cout << player->GetTypeAsString().c_str() << " post solve with " << other->GetTypeAsString().c_str() << std::endl;*/
}

void CollisionSystem::FindPlayer(b2Contact * contact, Entity *& player, Entity *& other)
{
	if (contact->GetFixtureA()->GetBody()->GetUserData() != "Obstacle" && contact->GetFixtureB()->GetBody()->GetUserData() != "Obstacle")
	{
		Entity* a = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* b = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (a->GetType() == EntityType::Player)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::Player)
		{
			player = b;
			other = a;
		}
		else if (a->GetType() == EntityType::AI)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::AI)
		{
			player = b;
			other = a;
		}
		else if (a->GetType() == EntityType::Flag)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::Flag)
		{
			player = b;
			other = a;
		}
		else if (a->GetType() == EntityType::Bullet)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::Bullet)
		{
			player = b;
			other = a;
		}
	}
	else
	{
		if (contact->GetFixtureA()->GetBody()->GetUserData() == "Obstacle")
		{
			player = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());
		}
		else
		{
			player = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
		}
	}
}

