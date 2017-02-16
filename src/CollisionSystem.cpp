#include "CollisionSystem.h"
#include <iostream>
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "ConstHolder.h"
#include <iostream>


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
	if (player != nullptr && other != nullptr)
		std::cout << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
}
void CollisionSystem::EndContact(b2Contact* contact)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);
	if (player != nullptr && other != nullptr)
		std::cout << player->GetTypeAsString().c_str() << " stopped colliding with " << other->GetTypeAsString().c_str() << std::endl;
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
		b2Vec2 normal = worldManifold.normal;
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));
		float speed = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
		float normalAngle = atan2f(-normal.y, normal.x) * (180.f / M_PI);
		float enterAngle = atan2f(-physics->yVelocity, physics->xVelocity) * (180.f / M_PI);

		if (normalAngle < 0.f)
			normalAngle += 360.f;
		if (enterAngle < 0.f)
			enterAngle += 360.f;

		float exitAngle = 2 * normalAngle - 180.f - enterAngle;

		if (exitAngle < 0.f)
			exitAngle += 360.f;
		exitAngle *= (M_PI / 180.f);

		float normalScaler;
		if (speed <= 1.f)
			normalScaler = 1.1f;
		else if (speed > 1.f)
			normalScaler = 1.1f * (1 / speed);
		else
			normalScaler = 1.1f;

		std::cout << "Normal Scaler: " << normalScaler << std::endl;

		//problem with 270/180/0/360 ???
		//apply torque as well
		std::cout << enterAngle << std::endl;
		physics->xVelocity = (cos(exitAngle) * speed * PLAYER_WALL_RESTITUTION) + -normal.x * normalScaler;
		physics->yVelocity = (-sin(exitAngle) * speed * PLAYER_WALL_RESTITUTION) + -normal.y * normalScaler;
		static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

		std::cout << player->GetTypeAsString().c_str() << " pre solve with " << other->GetTypeAsString().c_str() << std::endl;
	}
}

void CollisionSystem::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);
	if (player != nullptr && other != nullptr)
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
	else if(b->GetType() == EntityType::Player)
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

