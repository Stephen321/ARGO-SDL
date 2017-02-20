#pragma once

#include "System.h"


class DestructionSystem : public System
{
public:
							DestructionSystem(float update = 0.f);
							~DestructionSystem();

	void					Process(float dt = 0.f) override;

	void					DestroyEntity();

	std::vector<Entity*>&	GetEntitiesToBeDestroyed();

private:
	std::vector<Entity*>	_toBeDestroyed;
};

