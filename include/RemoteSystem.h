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


	bool							_startReadyTimer;
	bool							_startingGame;
	std::vector<float>				_serverDeltas;
	const float						PING_SEND_RATE = 0.2f;
	const int						PING_SEND_COUNT = 5;
	float							_pingTimer;
	bool							_pinging;
	const float						SYNC_RATE = 5.f; //re sync time every 5 seconds
};

#include "SystemManager.h"

