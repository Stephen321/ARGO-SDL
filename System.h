#pragma once

#include "Entity.h"

#include <map>

class System
{
public:
									System();
									~System();

	void							AddEntity(Entity* gameObject);
	void							RemoveEntity(Entity::Type tag, Entity* gameObject);

private:
	std::map<Entity::Type, 
		std::vector<Entity*>>		_gameObjects;
};

