#include "CollisionSystem.h"
#include <iostream>
#include "PhysicsComponent.h"
#include "CollisionComponent.h"


CollisionSystem::CollisionSystem(float updateRate)
	: System(updateRate)
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
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);

	//if contact->GetFixtureA()->GetBody()->GetType() == b2BodyType::b2_staticBody //static bodys manifold points are weird and linear velocity is null
	//b2Vec2 vel2 = static_cast<CollisionComponent*>(other->GetComponent(Component::Type::Collider))->body->GetLinearVelocityFromWorldPoint(worldManifold.points[0]); cant as static
	//b2Vec2 impactVelocity = vel1 - vel2;
	
	std::cout << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
}
void CollisionSystem::EndContact(b2Contact* contact)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);

	std::cout << player->GetTypeAsString().c_str() << " stopped colliding with " << other->GetTypeAsString().c_str() << std::endl;
}

void CollisionSystem::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	b2Vec2 normal = worldManifold.normal;
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));
	float speed = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
	physics->xVelocity += -normal.x * (speed / physics->maxVelocity) * 5.f;
	physics->yVelocity += -normal.y * (speed / physics->maxVelocity) * 5.f;
	static_cast<CollisionComponent*>(player->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity)); 
	

	std::cout << player->GetTypeAsString().c_str() << " pre solve with " << other->GetTypeAsString().c_str() << std::endl;
}

void CollisionSystem::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);

	std::cout << player->GetTypeAsString().c_str() << " post solve with " << other->GetTypeAsString().c_str() << std::endl;
}

void CollisionSystem::FindPlayer(b2Contact * contact, Entity *& player, Entity *& other)
{
	Entity* a = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
	Entity* b = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

	if (a->GetType() == EntityType::Player)
	{
		player = a;
		other = b;
	}
	else
	{
		player = b;
		other = a;
	}
}

