#pragma once

#include "System.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "NetworkHandler.h"
#include "Graph.h"

class RemoteSystem : public System
{
public:
	RemoteSystem(float updateRate, std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests);
	~RemoteSystem();

	void Initialize(Graph* waypoints);
	void Process(float dt = 0.f) override;

private:
	int									_flagHolderID;
	Graph*								_waypoints;
	std::vector<
		std::pair<EntityType,
		std::vector<float>>>& _creationRequests;
};

