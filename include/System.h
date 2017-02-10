#pragma once

#include "Entity.h"

#include <map>

class System
{
public:
	typedef std::map<Entity::Type,
		std::vector<Entity*>>::iterator EntityMapIterator;
									System(float updateRate = 0.f);
									~System();

	void							AddEntity(Entity* gameObject);
	void							RemoveEntity(Entity::Type tag, Entity* gameObject);

	virtual void					Process(float dt = 0.f);

protected:
	std::map<Entity::Type, 
		std::vector<Entity*>>		_entities;
	float							_updateRate;
	float							_timer;
	bool							_canUpdate;
};