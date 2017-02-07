#pragma once

#include "Component.h"


class BoundsComponent : public Component
{
public:
	BoundsComponent(float xPos, float yPos, int w, int h)
		: x(xPos)
		, y(yPos)
		, width(w)
		, height(h)
		, Component::Component(Component::Type::Bounds)
	{
	}

	~BoundsComponent()
	{
	}

public:
	float	x;
	float	y;
	
	int		width;
	int		height;
};
