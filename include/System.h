#pragma once

#include "Entity.h"

#include <map>

class System
{
public:
	typedef std::map<Entity::Type,
		std::vector<Entity*>>::iterator EntityMapIterator;
									System();
									~System();

	void							AddEntity(Entity* gameObject);
	void							RemoveEntity(Entity::Type tag, Entity* gameObject);

	virtual void					Process(float dt = 0.f) = 0;

protected:
	std::map<Entity::Type, 
		std::vector<Entity*>>		_entities;
};

