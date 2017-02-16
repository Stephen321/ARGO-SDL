#pragma once

#include "Component.h"
#include "GraphNode.h"

class MapComponent : public Component
{
public:
	MapComponent()
		: Component::Component(Component::Type::Map)	
	{
	}

	~MapComponent()
	{
	}

public:
	float radius;
	
};
