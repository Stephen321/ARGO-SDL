#include "WeaponSystem.h"

#include "TransformComponent.h"
#include "GunComponent.h"
#include "DestructionComponent.h"
#include "WeaponComponent.h"
#include "ConstHolder.h"



WeaponSystem::WeaponSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, float updateRate)
	: InteractionSystem(updateRate)
	, _interactionSystemEvents(events)
	, WEAPON_CREATED(InteractionSystemEvent::WeaponCreated)
	, BULLET_ADDITION(InteractionSystemEvent::WeaponAddBullets)
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

			WeaponComponent* weapon = static_cast<WeaponComponent*>(it->first->GetComponent(Component::Type::Weapon));
			
			if (weapon->fired)
			{
				GunComponent* gun = static_cast<GunComponent*>(it->second->GetComponent(Component::Type::Gun));

				if (gun->canFire)
				{
					gun->triggered = true;
				}

				weapon->fired = false;
			}

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
	WeaponBulletAddition();
}
void WeaponSystem::WeaponCreationEvent()
{
	if (!_interactionSystemEvents[WEAPON_CREATED].empty())
	{
		for (int i = 0; i < _interactionSystemEvents[WEAPON_CREATED].size(); i++)
		{
			if (_interactionSystemEvents[WEAPON_CREATED].at(i).second != nullptr)
			{
				for (int j = 0; j < _entities.size(); j++)
				{
					if (_entities.at(j).first == _interactionSystemEvents[WEAPON_CREATED].at(i).first)
					{
						static_cast<DestructionComponent*>(_entities.at(j).second->GetComponent(Component::Type::Destroy))->destroy = true;
						break;
					}
				}
				AddEntity(_interactionSystemEvents[WEAPON_CREATED].at(i).first, _interactionSystemEvents[WEAPON_CREATED].at(i).second);
				_interactionSystemEvents[WEAPON_CREATED].erase(_interactionSystemEvents[WEAPON_CREATED].begin() + i);
				i--;
			}
		}
	}
}

void WeaponSystem::WeaponBulletAddition()
{
	if (!_interactionSystemEvents[BULLET_ADDITION].empty())
	{
		for (int i = 0; i < _interactionSystemEvents[BULLET_ADDITION].size(); i++)
		{
			for (int j = 0; j < _entities.size(); j++)
			{
				if (_entities.at(j).first == _interactionSystemEvents[BULLET_ADDITION].at(i).first)
				{
					GunComponent* gun = static_cast<GunComponent*>(_entities.at(j).second->GetComponent(Component::Type::Gun));
					gun->ammo += AMMO[gun->id];
					break;
				}
			}

			_interactionSystemEvents[BULLET_ADDITION].erase(_interactionSystemEvents[BULLET_ADDITION].begin() + i);
			i--;
		}
	}
}

void WeaponSystem::RemoveEntity(Entity* gameObject, bool firstObject)
{
	if (firstObject)
	{
		for (int i = 0; i < _entities.size(); i++)
		{
			if (_entities.at(i).first == gameObject)
			{
				WeaponComponent* weapon = static_cast<WeaponComponent*>(_entities.at(i).first->GetComponent(Component::Type::Weapon));
				GunComponent* gun = static_cast<GunComponent*>(_entities.at(i).second->GetComponent(Component::Type::Gun));

				if (weapon->id == gun->id)
				{
					weapon->hasWeapon = false;
					weapon->id = -1;
				}
				
				_entities.erase(_entities.begin() + i);
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < _entities.size(); i++)
		{
			if (_entities.at(i).second == gameObject)
			{
				WeaponComponent* weapon = static_cast<WeaponComponent*>(_entities.at(i).first->GetComponent(Component::Type::Weapon));
				GunComponent* gun = static_cast<GunComponent*>(_entities.at(i).second->GetComponent(Component::Type::Gun));

				if (weapon->id == gun->id)
				{
					weapon->hasWeapon = false;
					weapon->id = -1;
				}

				_entities.erase(_entities.begin() + i);
				break;
			}
		}
	}
}