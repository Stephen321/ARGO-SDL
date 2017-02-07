#pragma once

#include "Component.h"


class PhysicsComponent : public Component
{
public:
	PhysicsComponent(float xV, float yV, float xA, float yA)
		: xVelocity(xV)
		, yVelocity(yV)
		, xAcceleration(xA)
		, yAcceleration(yA)
		, Component::Component(Component::Type::Physics)
	{
	}

	~PhysicsComponent()
	{
	}

public:
	float	xVelocity;
	float	yVelocity;

	float	xAcceleration;
	float	yAcceleration;
};