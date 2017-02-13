#include "InteractionSystem.h"



InteractionSystem::InteractionSystem(float updateRate)
	: _canUpdate(false)
	, _updateRate(updateRate)
{
}


InteractionSystem::~InteractionSystem()
{
}


void InteractionSystem::AddEntity(Entity* gameObject, Entity* otherObject)
{
	_entities[gameObject->GetType()].push_back(std::pair<Entity*, Entity*>(gameObject, otherObject));
}

void InteractionSystem::RemoveEntity(EntityType tag, Entity* gameObject)
{
	for (int i = 0; i < _entities[tag].size(); i++)
	{
		if (_entities[tag].at(i).first == gameObject)
		{
			_entities[tag].erase(_entities[tag].begin() + i);
			break;
		}
	}
}

void InteractionSystem::Process(float dt)
{
	_timer += dt;
	if (_timer >= _updateRate)
	{
		_timer = 0.f;
		_canUpdate = true;
	}
}