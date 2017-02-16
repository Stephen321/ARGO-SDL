#pragma once
#include "System.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

using namespace helper;
class WorldSystem : public System
{
public:
	WorldSystem(float updateRate = 0.f);
	~WorldSystem();

	void Initialize(Graph* map);
	void Process(float dt = 0.f) override;

private:
	Graph* _map;
};

