#include "WeaponSystem.h"

#include "BoundsComponent.h"



WeaponSystem::WeaponSystem()
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
				BoundsComponent* bound1 = static_cast<BoundsComponent*>(e.first->GetComponent(Component::Type::Bounds));
				BoundsComponent* bound2 = static_cast<BoundsComponent*>(e.second->GetComponent(Component::Type::Bounds));

				bound2->rect.x = bound1->rect.x;// +(cos(bound1->angle * M_PI / 180.0f) * (bound1->origin.x + bound2->origin.x));
				bound2->rect.y = bound1->rect.y;// +(sin(bound1->angle * M_PI / 180.0f) * (bound1->origin.y + bound2->origin.y));

				bound2->angle = bound1->angle;
			}
		}
	}
}
