#pragma once

#include "Component.h"
#include "Box2D\Box2D.h"


class CollisionComponent : public Component
{
public:
	CollisionComponent(b2Body* b)
		: body(b)
		, Component::Component(Component::Type::Bounds)
	{
	}

	~CollisionComponent()
	{
	}

public:
	b2Body* body;
};

