#pragma once

#include "Component.h"
#include "GraphNode.h"


class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		PathFinderUpdateRate(2.f),
		pathfinderUpdateTimer(0),
		startNode(nullptr),
		nextNode(nullptr)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;
	float PathFinderUpdateRate;
	float pathfinderUpdateTimer;
	GraphNode* startNode;
	GraphNode* nextNode;
};
