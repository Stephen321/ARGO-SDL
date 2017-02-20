#pragma once

#include "Component.h"
#include "GraphNode.h"

class WaypointComponent : public Component
{
public:
	WaypointComponent(float r) :
		Component::Component(Component::Type::Map),
		radius(r)
	{
	}

	~WaypointComponent()
	{
	}

public:
	float radius;
	
};
