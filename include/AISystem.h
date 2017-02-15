#pragma once

#include "System.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

using namespace helper;
class AISystem : public System
{
public:
	AISystem(float updateRate = 0.f);
	~AISystem();

	void Initialize(Graph<string>* map);
	void Process(float dt = 0.f) override;

private:
	Graph<string>* _map;
};

