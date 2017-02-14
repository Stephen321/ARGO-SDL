#include "ControlSystem.h"

#include "ControlComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "GunComponent.h"

#include <iostream>

ControlSystem::ControlSystem(float updateRate)
	: System(updateRate)
{
}

void ControlSystem::Initialize(Camera2D::Camera* camera)
{
	_camera = camera;
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
			TransformComponent* transform = static_cast<TransformComponent*>(_turrets[i]->GetComponent(Component::Type::Transform));

			//initialize mouuse position
			SDL_Point mouse;
			SDL_GetMouseState(&mouse.x, &mouse.y);

			//calculate mouse position to world
			Camera2D::Point convertedPoint = _camera->screenToWorld(Camera2D::Point(mouse.x, mouse.y));

			Camera2D::Point difference = { convertedPoint.x - transform->rect.x + transform->origin.x, convertedPoint.y - transform->rect.y + transform->origin.y };
			transform->angle = atan2(difference.y, difference.x) * 180.f / M_PI;
		}
	}
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
