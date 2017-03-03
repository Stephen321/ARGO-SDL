#pragma once

#include "System.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "NetworkHandler.h"
#include "Graph.h"

class SystemManager;
class RemoteSystem : public System
{
public:
	RemoteSystem(float updateRate, std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests);
	~RemoteSystem();

	void Initialize(Graph* waypoints, SystemManager* systemManager);
	void Process(float dt = 0.f) override;

private:
	int									_flagHolderID;
	Graph*								_waypoints;
	SystemManager*						_systemManager;
	std::vector<
		std::pair<EntityType,
		std::vector<float>>>& _creationRequests;
};

#include "SystemManager.h"

