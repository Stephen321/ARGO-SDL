#pragma once

#include "Component.h"
#include "GraphNode.h"

enum class AIState
{
	Entry,
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
		pathFinderUpdateRate(2.0f),
		pathfinderUpdateTimer(0),
		nextNode(nullptr),
		flagDetectionRange(false),
		state(AIState::Entry)
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

	AIState state;

};
