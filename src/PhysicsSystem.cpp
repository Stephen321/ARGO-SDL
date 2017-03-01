#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "StatusEffectComponent.h"
#include "DestructionComponent.h"
#include "RemoteComponent.h"

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
				StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));

				//if (e->GetType() == EntityType::AI)
				//{
				//	std::cout << "AI velocity: " << physics->xVelocity << " , y: " << physics->yVelocity << std::endl;
				//}
				float maxVelocity = physics->maxVelocity;

				float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
				float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;


				float xAccel = physics->xAcceleration;
				float yAccel = physics->yAcceleration;

				if (statusEffects != nullptr && statusEffects->speedUp)
				{
					xAccel += xAccel;
					yAccel += yAccel;
				}

				physics->xVelocity += (xDrag + (physics->xDir * xAccel)) * dt;//change dt to _updateRate?//maybe?
				physics->yVelocity += (yDrag + (physics->yDir * yAccel)) * dt;

				if ((e->GetType() == EntityType::AI || e->GetType() == EntityType::Player) && physics->xVelocity + physics->yVelocity != 0)
				{
					transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
				}

				float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
				if (currentVelocity > maxVelocity)
				{
					physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
					physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
				}

				if (e->GetType() == EntityType::Bullet)
				{
					if (std::abs(physics->xVelocity) < 0.5f) 
					{ 
						physics->xVelocity = 0.f; 
					}
					if (std::abs(physics->yVelocity) < 0.5f) 
					{ 
						physics->yVelocity = 0.f; 
					}

					if (physics->xVelocity + physics->yVelocity == 0)
					{
						static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy))->destroy = true;
					}
				}
				else
				{
					if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
					if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }
				}

				if (collider->body->IsActive())
				{
					collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

					transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
					transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
				}

				if (e->GetType() == EntityType::Player)
				{
					physics->xDir = 0;
					physics->yDir = 0;
				}
			}
		}
	}
}
