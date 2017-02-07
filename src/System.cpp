#include "System.h"



System::System()
{
}


System::~System()
{
}


void System::AddEntity(Entity* gameObject)
{
	_gameObjects[gameObject->GetType()].push_back(gameObject);
}

void System::RemoveEntity(Entity::Type tag, Entity* gameObject)
{
	for (int i = 0; i < _gameObjects[tag].size(); i++)
	{
		if (_gameObjects[tag].at(i) == gameObject)
		{
			_gameObjects[tag].erase(_gameObjects[tag].begin() + i);
			break;
		}
	}
}