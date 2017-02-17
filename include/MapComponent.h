#pragma once

#include "Component.h"
#include "GraphNode.h"

class MapComponent : public Component
{
public:
	MapComponent(float r) :
		Component::Component(Component::Type::Map),
		radius(r)
	{
	}

	~MapComponent()
	{
	}

public:
	float radius;
	
};
