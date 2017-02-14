#pragma once

#include "System.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

class AISystem : public System
{
public:
	AISystem(Graph<string, int, int>* map);
	~AISystem();

	void Process(float dt = 0.f) override;

private:
	Graph<string, int, int>* _map;
};

