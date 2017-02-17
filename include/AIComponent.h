#pragma once

#include "Component.h"
#include "GraphNode.h"


class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<GraphNode*>()),
		updateRate(5.f),
		updateTimer(0),
		callAstar(false)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<GraphNode*> path;
	float updateRate;
	float updateTimer;
	bool callAstar;
};
