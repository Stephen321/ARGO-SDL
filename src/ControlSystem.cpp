#include "ControlSystem.h"

#include "PhysicsComponent.h"
#include "BoundsComponent.h"

ControlSystem::ControlSystem(float updateRate)
	: System(updateRate)
{
	
}


ControlSystem::~ControlSystem()
{
}

void ControlSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
	}
}

void ControlSystem::MovePlayer(int x, int y, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));
	physics->xVelocity = x * 5;
	physics->yVelocity = y * 5;

	BoundsComponent* bounds = static_cast<BoundsComponent*>(entity->GetComponent(Component::Type::Bounds));
	bounds->x += physics->xVelocity;
	bounds->y += physics->yVelocity;
	bounds->rect.x = (int)bounds->x;
	bounds->rect.y = (int)bounds->y;
}
