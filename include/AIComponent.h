#pragma once

#include "Component.h"
#include "GraphNode.h"


class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		pathFinderUpdateRate(1.f),
		pathfinderUpdateTimer(0),
		nextNode(nullptr),
		inFlagRange(false)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;
	float pathFinderUpdateRate;
	float pathfinderUpdateTimer;
	GraphNode* nextNode;

	bool inFlagRange;
};
