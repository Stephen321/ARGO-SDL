#pragma once

#include "Component.h"
#include "GraphNode.h"
enum class AIState
{
	SeekFlag,
	SeekCheckpoint,
	SeekPowerUp,
};

class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		pathFinderUpdateRate(1.f),
		pathfinderUpdateTimer(0),
		nextNode(nullptr),
		flagDetectionRange(false),
		state(AIState::SeekFlag),
		closestNodeFlag(false),
		updateAStarFlag(false)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;
	float pathFinderUpdateRate;
	float pathfinderUpdateTimer;

	GraphNode* destNode;
	GraphNode* nextNode;

	bool flagDetectionRange;

	bool closestNodeFlag;
	bool updateAStarFlag;

	AIState state;
};
