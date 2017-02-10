
#include "PhysicsSystem.h"
#include "BoundsComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"

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
				float maxVelocity = physics->maxVelocity;

				float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG * dt : 0.f;
				float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG * dt: 0.f;

				physics->xVelocity += (xDrag + (physics->xDir * physics->xAcceleration)) * dt;//change dt to _updateRate?
				physics->yVelocity += (yDrag + (physics->yDir * physics->yAcceleration)) * dt;

				float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
				if (currentVelocity > maxVelocity)
				{
					physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
					physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
				}

				if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.1f) { physics->xVelocity = 0.f; }
				if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.1f) { physics->yVelocity = 0.f; }

				collision->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity)); 
				
				bounds->rect.x = (int)metersToPixels(collision->body->GetPosition().x);
				bounds->rect.y = (int)metersToPixels(collision->body->GetPosition().y);

				physics->xDir = 0;
				physics->yDir = 0;
				std::cout << "Velcoity.x: " << collision->body->GetLinearVelocity().x << " y: " << collision->body->GetLinearVelocity().y << std::endl;
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
