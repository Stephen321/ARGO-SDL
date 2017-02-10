#include "ControlSystem.h"

#include "ControlComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"

#include <iostream>

ControlSystem::ControlSystem(Camera2D::Camera* camera, float updateRate)
	: System(updateRate)
	, _camera(camera)
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

		for (int i = 0; i < _turrets.size(); i++)
		{
			TransformComponent* bound = static_cast<TransformComponent*>(_turrets[i]->GetComponent(Component::Type::Transform));

			//initialize mouuse position
			SDL_Point mouse;
			SDL_GetMouseState(&mouse.x, &mouse.y);

			//calculate mouse position to world
			Camera2D::Point convertedPoint = _camera->screenToWorld(Camera2D::Point(mouse.x, mouse.y));

			Camera2D::Point difference = { convertedPoint.x - bound->rect.x + bound->origin.x, convertedPoint.y - bound->rect.y + bound->origin.y };
			bound->angle = atan2(difference.y, difference.x) * 180.f / M_PI;
		}
	}
}

void ControlSystem::MovePlayer(int x, int y, Entity*& entity)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(entity->GetComponent(Component::Type::Physics));

	physics->xVelocity += x * physics->xAcceleration;
	physics->yVelocity += y * physics->yAcceleration;

	physics->xDir = x;
	physics->yDir = y;
}

void ControlSystem::OnEvent(Event evt)
{

}


void ControlSystem::AddTurret(Entity* entity)
{
	_turrets.push_back(entity);
}
void ControlSystem::RemoveTurret(Entity* entity)
{
	for (int i = 0; i < _turrets.size(); i++)
	{
		if (_turrets.at(i) == entity)
		{
			_turrets.erase(_turrets.begin() + i);
			break;
		}
	}
}