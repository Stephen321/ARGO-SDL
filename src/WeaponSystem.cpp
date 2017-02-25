#include "WeaponSystem.h"

#include "TransformComponent.h"



WeaponSystem::WeaponSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, float updateRate)
	: InteractionSystem(updateRate)
	, _interactionSystemEvents(events)
	, WEAPON_CREATED(InteractionSystemEvent::WeaponCreated)
{
}


WeaponSystem::~WeaponSystem()
{
}


void WeaponSystem::Initialize(Camera2D::Camera* camera)
{
	_camera = camera;
}

void WeaponSystem::Process(float dt)
{
	InteractionSystem::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		ListenForEvents();

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			TransformComponent* transform1 = static_cast<TransformComponent*>(it->first->GetComponent(Component::Type::Transform));
			TransformComponent* transform2 = static_cast<TransformComponent*>(it->second->GetComponent(Component::Type::Transform));

			transform2->rect.x = transform1->rect.x;
			transform2->rect.y = transform1->rect.y;

			if (it->first->GetType() == EntityType::Player)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(it->second->GetComponent(Component::Type::Transform));

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
}

void WeaponSystem::ListenForEvents()
{
	WeaponCreationEvent();
}
void WeaponSystem::WeaponCreationEvent()
{
	if (!_interactionSystemEvents[WEAPON_CREATED].empty())
	{
		for (int i = 0; i < _interactionSystemEvents[WEAPON_CREATED].size(); i++)
		{
			if (_interactionSystemEvents[WEAPON_CREATED].at(i).second != nullptr)
			{
				AddEntity(_interactionSystemEvents[WEAPON_CREATED].at(i).first, _interactionSystemEvents[WEAPON_CREATED].at(i).second);
				_interactionSystemEvents[WEAPON_CREATED].erase(_interactionSystemEvents[WEAPON_CREATED].begin() + i);
				i--;
			}
		}
	}
}
