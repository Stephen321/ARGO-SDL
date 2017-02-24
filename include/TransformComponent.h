#pragma once

#include "Component.h"
#include "SDL_rect.h"


class TransformComponent : public Component
{
public:
	TransformComponent(float xPos, float yPos, int w, int h)
		: Component::Component(Component::Type::Transform)
		, rect({ (int)xPos, (int)yPos, (int)w, (int)h })
		, origin({ (int)(w*0.5f), (int)(h*0.5f) })
		, scaleX(1.f)
		, scaleY(1.f)
		, angle(0)
	{
	}

	TransformComponent(float xPos, float yPos, int w, int h, float sX, float sY)
		: Component::Component(Component::Type::Transform)
		, rect({ (int)xPos, (int)yPos, (int)w, (int)h })
		, origin({ (int)(w*0.5f), (int)(h*0.5f) })
		, scaleX(sX)
		, scaleY(sY)
		, angle(0)
	{
	}

	TransformComponent(float xPos, float yPos, int w, int h, float sX, float sY, float a)
		: Component::Component(Component::Type::Transform)
		, rect({(int)xPos, (int)yPos, (int)w, (int)h})
		, origin({ (int)(w*0.5f), (int)(h*0.5f)})
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}

	TransformComponent(float xPos, float yPos, int w, int h, int oX, int oY, float sX, float sY, float a)
		: Component::Component(Component::Type::Transform)
		, rect({ (int)xPos, (int)yPos, (int)w, (int)h })
		, origin({ oX, oY })
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}
	
	TransformComponent(SDL_Rect r)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin({ (int)(r.w*0.5f), (int)(r.h*0.5f) })
		, scaleX(1.f)
		, scaleY(1.f)
		, angle(0)
	{
	}

	TransformComponent(SDL_Rect r, float sX, float sY)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin({ (int)(r.w*0.5f), (int)(r.h*0.5f) })
		, scaleX(sX)
		, scaleY(sY)
		, angle(0)
	{
	}

	TransformComponent(SDL_Rect r, float sX, float sY, float a)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin({ (int)(r.w*0.5f), (int)(r.h*0.5f) })
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}

	TransformComponent(SDL_Rect r, SDL_Point o, float sX, float sY, float a)
		: Component::Component(Component::Type::Transform)
		, rect(r)
		, origin(o)
		, scaleX(sX)
		, scaleY(sY)
		, angle(a)
	{
	}

	~TransformComponent()
	{
	}

public:
	SDL_Rect	rect;
	SDL_Point	origin;

	float		scaleX;
	float		scaleY;

	float		angle;
};
