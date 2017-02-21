#pragma once
#include "System.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

using namespace helper;
class WaypointSystem : public System
{
public:
	WaypointSystem(float updateRate = 0.f);
	~WaypointSystem();

	void Initialize(Graph* waypoint);
	void Process(float dt = 0.f) override;

private:
	Graph* _waypoints;
};