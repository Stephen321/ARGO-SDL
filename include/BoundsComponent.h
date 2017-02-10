#pragma once

#include "Component.h"
#include "SDL_rect.h"


class BoundsComponent : public Component
{
public:
	BoundsComponent(float xPos, float yPos, int w, int h, float sX, float sY, float a)
		: Component::Component(Component::Type::Bounds)
		, rect({(int)xPos, (int)yPos, (int)w, (int)h})
		, origin({ (int)(w*0.5f), (int)(h*0.5f)})
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}

	BoundsComponent(float xPos, float yPos, int w, int h, int oX, int oY, float sX, float sY, float a)
		: Component::Component(Component::Type::Bounds)
		, rect({ (int)xPos, (int)yPos, (int)w, (int)h })
		, origin({ oX, oY })
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}
	
	BoundsComponent(SDL_Rect r, float sX, float sY, float a)
		: Component::Component(Component::Type::Bounds)
		, rect(r)
		, origin({ (int)(r.w*0.5f), (int)(r.h*0.5f) })
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}

	BoundsComponent(SDL_Rect r, SDL_Point o, float sX, float sY, float a)
		: Component::Component(Component::Type::Bounds)
		, rect(r)
		, origin(o)
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}

	~BoundsComponent()
	{
	}

public:
	SDL_Rect	rect;
	SDL_Point	origin;

	float		scaleX;
	float		scaleY;

	float		angle;
};
