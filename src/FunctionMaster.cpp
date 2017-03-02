#include "FunctionMaster.h"

#include "PhysicsComponent.h"
#include "StatusEffectComponent.h"


void FunctionMaster::MoveHorizontal(int dir, Entity*& entity)
{
	StatusEffectComponent* effects = static_cast<StatusEffectComponent*>(entity->GetComponent(Component::Type::StatusEffect));

	if (!effects->staggered)
	{
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

		physics->xDir = dir;
	}
}

void FunctionMaster::MoveVertical(int dir, Entity*& entity)
{
	StatusEffectComponent* effects = static_cast<StatusEffectComponent*>(entity->GetComponent(Component::Type::StatusEffect));

	if (!effects->staggered)
	{
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

		physics->yDir = dir;
	}
}