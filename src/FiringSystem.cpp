#include "FiringSystem.h"
#include "GunComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"


FiringSystem::FiringSystem(float updateRate)
	: System(updateRate)
{
}


FiringSystem::~FiringSystem()
{
}

void FiringSystem::Initialize(std::vector<Entity*>*& entities, EntityFactory*& entityFactory, BodyFactory*& bodyFactory)
{
	_gameEntities = entities;
	_entityFactory = entityFactory;
	_bodyFactory = bodyFactory;
}

void FiringSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				GunComponent* gun = static_cast<GunComponent*>(e->GetComponent(Component::Type::Gun));

				if (gun->triggered)
				{
					gun->fireTimer = gun->maxTimer;
					gun->triggered = false;
					gun->canFire = false;
					gun->ammo--;

					TransformComponent* gunTransform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

					Entity* bullet = _entityFactory->CreateEntity(Entity::Type::Bullet);

					TransformComponent* transform = static_cast<TransformComponent*>(bullet->GetComponent(Component::Type::Transform));
					ColliderComponent* collider = static_cast<ColliderComponent*>(bullet->GetComponent(Component::Type::Collider));
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(bullet->GetComponent(Component::Type::Physics));

					transform->rect.x = gunTransform->rect.x * gunTransform->scaleX;
					transform->rect.y = gunTransform->rect.y * gunTransform->scaleY;

					collider->body = _bodyFactory->CreateBoxBody(
						  b2BodyType::b2_dynamicBody
						, b2Vec2(gunTransform->rect.x + (cos(gunTransform->angle * M_PI / 180.0f)) * (gunTransform->origin.x * gunTransform->scaleX), gunTransform->rect.y + (sin(gunTransform->angle * M_PI / 180.0f)) * (gunTransform->origin.x * gunTransform->scaleY))
						, gunTransform->angle
						, b2Vec2((transform->rect.w * 0.5f) * transform->scaleX, (transform->rect.h * 0.5f) * transform->scaleY)
						, true);

					collider->body->SetTransform(collider->body->GetPosition(), gunTransform->angle);

					physics->xVelocity = cos(gunTransform->angle * M_PI / 180.0f) * (physics->maxVelocity);
					physics->yVelocity = sin(gunTransform->angle * M_PI / 180.0f) * (physics->maxVelocity);

					transform->angle = gunTransform->angle;
				}						 
			}
		}
	}
}