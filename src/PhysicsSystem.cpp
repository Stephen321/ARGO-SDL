
#include "PhysicsSystem.h"
#include "BoundsComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ConstHolder.h"

#include <iostream>

PhysicsSystem::PhysicsSystem(float updateRate)
	: System(updateRate)
{
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
				BoundsComponent* bounds = static_cast<BoundsComponent*>(e->GetComponent(Component::Type::Bounds));
				CollisionComponent* collision = static_cast<CollisionComponent*>(e->GetComponent(Component::Type::Collider));
				
				bounds->rect.x = (int)collision->body->GetPosition().x;
				bounds->rect.y = (int)collision->body->GetPosition().y;

				//physics->xVelocity *= DRAG;
				//physics->yVelocity *= DRAG;
				if (std::abs(physics->xVelocity) <= 0.2f) { physics->xVelocity = 0; }
				if (std::abs(physics->yVelocity) <= 0.2f) { physics->yVelocity = 0; }

				collision->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

				std::cout << "Velcoity: " << physics->xVelocity << std::endl;
			}
		}
	}
}

////bounds->x += physics->xVelocity;
////bounds->y += physics->yVelocity;
//
//bounds->rect.x = (int)collision->body->GetPosition().x;
//bounds->rect.y = (int)collision->body->GetPosition().y;
//
//collision->body->SetLinearVelocity(
//	b2Vec2(
//		physics->xVelocity,
//		physics->yVelocity
//	)
//);
//
//collision->body->ApplyForceToCenter(collision->body->GetPosition(), true);
//
////	physics->xVelocity *= DRAG;
////	physics->yVelocity *= DRAG;
//
//std::cout << "Velcoity1: " << physics->xVelocity << std::endl;
//std::cout << "Velcoity2: " << collision->body->GetLinearVelocity().x << std::endl;
//
//if (std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0; }
//if (std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0; }
