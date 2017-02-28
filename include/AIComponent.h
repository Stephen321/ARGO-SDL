#pragma once

#include "Component.h"
#include "GraphNode.h"


class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		updateRate(2.f),
		updateTimer(0),
		callAstar(false)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;

	GraphNode*			startingNode;

	float				updateRate;
	float				updateTimer;

	bool				callAstar;
};
