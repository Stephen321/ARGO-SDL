#include "PhysicsSystem.h"
#include "BoundsComponent.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"


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

				bounds->x += physics->xVelocity;
				bounds->y += physics->yVelocity;
				bounds->rect.x = (int)bounds->x;
				bounds->rect.y = (int)bounds->y;

				physics->xVelocity *= DRAG;
				physics->yVelocity *= DRAG;
			}
		}
	}
}
