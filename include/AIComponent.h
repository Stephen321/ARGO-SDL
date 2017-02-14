#pragma once

#include "Component.h"
#include "GraphNode.h"
typedef GraphNode<string, int, int> Node;

class AIComponent : public Component
{
public:
	AIComponent()
		: Component::Component(Component::Type::AI),
		path(vector<Node*>()),
		updateRate(5.f),
		updateTimer(0),
		callAstar(false)
	{
	}

	~AIComponent()
	{
	}

public:
	vector<Node*> path;
	float updateRate;
	float updateTimer;
	bool callAstar;
};
