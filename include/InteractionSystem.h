#pragma once

#include "Entity.h"

#include <map>


class InteractionSystem
{
public:
	typedef std::map<EntityType, std::vector<std::pair<Entity*, Entity*>>>::iterator EntityMapIterator;

	InteractionSystem(float updateRate = 0.f);
	~InteractionSystem();

	void							AddEntity(Entity* gameObject, Entity* otherObject);
	void							RemoveEntity(EntityType tag, Entity* gameObject);

	virtual void					Process(float dt = 0.f);

protected:
	std::map<EntityType,
		std::vector<
			std::pair<Entity*, 
					Entity*>>>		_entities;
	float							_updateRate;
	float							_timer;
	bool							_canUpdate;
};

