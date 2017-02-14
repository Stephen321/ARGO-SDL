#include "FunctionMaster.h"

#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GunComponent.h"

void FunctionMaster::MoveHorizontal(int dir, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

	physics->xDir = dir;

	RotateEntity(entity);
}
void FunctionMaster::MoveVertical(int dir, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

	physics->yDir = dir;

	RotateEntity(entity);
}

void FunctionMaster::FireBullet(Entity*& entity)
{
	GunComponent* gun = static_cast<GunComponent*>(entity->GetComponent(Component::Type::Gun));
	TransformComponent* transform = static_cast<TransformComponent*>(entity->GetComponent(Component::Type::Transform));

	if (gun->ammo > 0)
	{
		gun->triggered = true;
	}
}

void FunctionMaster::RotateEntity(Entity* entity)
{
	TransformComponent* transform = static_cast<TransformComponent*>(entity->GetComponent(Component::Type::Transform));
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));
	ColliderComponent* collider = static_cast<ColliderComponent*>(entity->GetComponent(Component::Type::Collider));

	//transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
	transform->angle = atan2(physics->yDir, physics->xDir) * 180.f / M_PI;

	float dToR = M_PI / 180;
	float angle = transform->angle * dToR;

	collider->body->SetTransform(collider->body->GetPosition(), angle);
}