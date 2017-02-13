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
void ControlSystem::MoveHorizontal(int dir, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));
	physics->xDir = dir;
}

void ControlSystem::MoveVertical(int dir, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));
	physics->yDir = dir;
}