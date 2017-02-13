#pragma once

#include "Component.h"
#include "Box2D\Box2D.h"


class CollisionComponent : public Component
{
public:
	CollisionComponent()
		: body(nullptr)
		, Component::Component(Component::Type::Collider)
	{
	}

	~CollisionComponent()
	{
	}

public:
	b2Body* body;
};

