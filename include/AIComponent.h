#pragma once

#include "Component.h"
#include "GraphNode.h"

enum class AIState
{
	Entry,
	Chase,
	SeekFlag,
	Camp,
	SeekCheckpoint,
	SeekPowerUp,
};

class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		pathfinderUpdateTimer(0),
		nextNode(nullptr),
		state(AIState::Entry),
		lastEntityWithFlag(nullptr),
		avoidanceColliderTimer(0),
		avoidanceForce(0)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;
	float pathfinderUpdateTimer;
	float avoidanceColliderTimer;
	float avoidanceForce;
	GraphNode* destNode;
	GraphNode* nextNode;

	AIState state;

	Entity* lastEntityWithFlag;
};
