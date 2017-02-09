#include "ControlSystem.h"

#include "ControlComponent.h"
#include "PhysicsComponent.h"

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
	physics->xVelocity += x * physics->xAcceleration;
	physics->yVelocity += y * physics->yAcceleration;
}
