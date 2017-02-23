#include "FunctionMaster.h"

#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "GunComponent.h"

void FunctionMaster::MoveHorizontal(int dir, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

	physics->xDir = dir;
}
void FunctionMaster::MoveVertical(int dir, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

	physics->yDir = dir;
}

void FunctionMaster::FireBullet(Entity*& entity)
{
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	if (entity != nullptr && entity->GetComponents().size() > 0 && entity->GetComponents().size() < 10)
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	//////////IF CHECK ONLY FOR NOW////////////////////////////////////////////////////////////////////////////
	{
		GunComponent* gun = static_cast<GunComponent*>(entity->GetComponent(Component::Type::Gun));

		if (gun->ammo > 0)
		{
			gun->triggered = true;
		}
	}
}