#pragma once

#include "Component.h"


class BoundsComponent : public Component
{
public:
	BoundsComponent(float xPos, float yPos, int w, int h)
		: Component::Component(Component::Type::Bounds)
		, x(xPos)
		, y(yPos)
		, width(w)
		, height(h)
		, rect({(int)xPos, (int)yPos, (int)w, (int)h})
	{
	}

	~BoundsComponent()
	{
	}

public:
	SDL_Rect rect;
	float	x;
	float	y;
	
	int		width;
	int		height;
};
