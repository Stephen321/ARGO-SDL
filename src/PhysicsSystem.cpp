
#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
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
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

				float maxVelocity = physics->maxVelocity;

				float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
				float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;

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

				collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity)); 
				
				transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
				transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);

				physics->xDir = 0;
				physics->yDir = 0;
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
