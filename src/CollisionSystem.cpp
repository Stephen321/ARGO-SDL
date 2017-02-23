#include "CollisionSystem.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "ConstHolder.h"

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
		//for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		//{
		//	for (Entity* e : (*it).second)
		//	{

		//	}
		//}
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
			std::cout << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
		}
	}
}
void CollisionSystem::EndContact(b2Contact* contact)
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
			std::cout << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
		}
	}
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
		float e = 0.6f; //elasticity restitution

		float xVelocityTemp = e1Physics->xVelocity * e;
		float yVelocityTemp = e1Physics->yVelocity * e;

		e1Physics->xVelocity = e2Physics->xVelocity * e;
		e1Physics->yVelocity = e2Physics->yVelocity * e;

		e2Physics->xVelocity = xVelocityTemp;
		e2Physics->yVelocity = yVelocityTemp;
	}
	else
	{
		float e = 0.8f; //elasticity restitution

		if (e1speed != 0)
		{
			e2Physics->xVelocity = e1Physics->xVelocity * e;
			e2Physics->yVelocity = e1Physics->yVelocity * e;

			e1Physics->xVelocity *= (1 - e);
			e1Physics->yVelocity *= (1 - e);
		}
		else if (e2speed != 0)
		{
			e1Physics->xVelocity = e2Physics->xVelocity * e;
			e1Physics->yVelocity = e2Physics->yVelocity * e;

			e2Physics->xVelocity *= (1 - e);
			e2Physics->yVelocity *= (1 - e);
		}
	}

	static_cast<ColliderComponent*>(e1->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(e1Physics->xVelocity, e1Physics->yVelocity));
	static_cast<ColliderComponent*>(e2->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(e2Physics->xVelocity, e2Physics->yVelocity));
}

void CollisionSystem::CharacterObstacleBounce(b2WorldManifold& worldManifold, Entity*& player)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));

	float e = 0.8f; //elasticity restitution
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
			speed *= e;
			physics->xVelocity = (xDirectionNew / directionlenght) * speed;
			physics->yVelocity = (yDirectionNew / directionlenght) * speed;
		}
		else
		{
			physics->xVelocity = -physics->xVelocity * e;
			physics->yVelocity = -physics->yVelocity * e;
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

