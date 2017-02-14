#include "GunSystem.h"
#include "GunComponent.h"


GunSystem::GunSystem(float updateRate)
	: System(updateRate)
{
}


GunSystem::~GunSystem()
{
}



void GunSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				GunComponent* gun = static_cast<GunComponent*>(e->GetComponent(Component::Type::Gun));

				if (!gun->canFire)
				{
					gun->fireTimer -= dt;

					if (gun->fireTimer < 0)
					{
						gun->canFire = true;
					}
				}
			}
		}
	}
}
