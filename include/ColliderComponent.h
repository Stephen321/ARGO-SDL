#pragma once

#include "Component.h"
#include "Box2D\Box2D.h"


class ColliderComponent : public Component
{
public:
	ColliderComponent()
		: body(nullptr)
		, Component::Component(Component::Type::Collider)
	{
	}

	~ColliderComponent()
	{
	}

public:
	b2Body* body;
};
