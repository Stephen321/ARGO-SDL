#include "System.h"



System::System()
{
}


System::~System()
{
}


void System::AddEntity(Entity* gameObject)
{
	_entities[gameObject->GetType()].push_back(gameObject);
}

void System::RemoveEntity(Entity::Type tag, Entity* gameObject)
{
	for (int i = 0; i < _entities[tag].size(); i++)
	{
		if (_entities[tag].at(i) == gameObject)
		{
			_entities[tag].erase(_entities[tag].begin() + i);
			break;
		}
	}
}