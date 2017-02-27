#pragma once

#include "System.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

#include "AIComponent.h"
#include "TransformComponent.h" 
#include "FlagComponent.h"

using namespace helper;
class AISystem : public System
{
public:
	AISystem(float updateRate = 0.f);
	~AISystem();

	void Initialize(Graph* waypoints);
	void Process(float dt = 0.f) override;

private:
	Graph* _waypoints;
	GraphNode* _flagNode;
	vector<GraphNode*> _checkpointNode;


	void Entry(AIComponent* ai, TransformComponent* t);
	void SeekFlag(AIComponent* ai, FlagComponent* f, TransformComponent* t);
	void SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t);
	void SeekPowerup()
	{

	}

	void UsePowerup()
	{

	}

	void AStar(AIComponent* ai);
	GraphNode* DetermineClosestNode(TransformComponent* t);

	void FlagPrediction()
	{

	}

	void Shoot()
	{

	}
};

