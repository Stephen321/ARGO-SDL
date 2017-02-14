#include "WeaponSystem.h"

#include "TransformComponent.h"



WeaponSystem::WeaponSystem(float updateRate)
	: InteractionSystem(updateRate)
{
}


WeaponSystem::~WeaponSystem()
{
}


void WeaponSystem::Process(float dt)
{
	InteractionSystem::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (std::pair<Entity*, Entity*> e : (*it).second)
			{
				TransformComponent* transform1 = static_cast<TransformComponent*>(e.first->GetComponent(Component::Type::Transform));
				TransformComponent* transform2 = static_cast<TransformComponent*>(e.second->GetComponent(Component::Type::Transform));

				transform2->rect.x = transform1->rect.x;
				transform2->rect.y = transform1->rect.y;
			}
		}
	}
}
