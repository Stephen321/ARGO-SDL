#pragma once

#include "System.h"
#include "BodyFactory.h"

class EntityFactory;

class FiringSystem : public System
{
public:
							FiringSystem(float updateRate = 0.f);
							~FiringSystem();

	void					Initialize(std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory);

	void					Process(float dt = 0.f) override;

private:
	std::vector<Entity*>*	_gameEntities;

	EntityFactory*			_entityFactory;
	BodyFactory*			_bodyFactory;
};

#include "EntityFactory.h"