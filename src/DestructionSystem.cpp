#include "DestructionSystem.h"
#include "DestructionComponent.h"


DestructionSystem::DestructionSystem(float update)
{
}


DestructionSystem::~DestructionSystem()
{
}


void DestructionSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				DestructionComponent* destroy = static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy));

				if (destroy->destroy)
				{
					_toBeDestroyed.push_back(e);
				}
			}
		}
	}
}

void DestructionSystem::DestroyEntity()
{
	if (!_toBeDestroyed.empty())
	{
		int index = _toBeDestroyed.size() - 1;

		delete _toBeDestroyed.at(index);
		_toBeDestroyed.at(index) = nullptr;

		_toBeDestroyed.pop_back();
	}
}

std::vector<Entity*>& DestructionSystem::GetEntitiesToBeDestroyed()
{
	return _toBeDestroyed;
}