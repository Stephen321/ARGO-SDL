#pragma once

#include "Component.h"

#include "SDL_image.h"


class SpriteComponent : public Component
{
public:
	SpriteComponent(SDL_Texture* texture)
		: _texture(texture)
		, Component::Component(Component::Type::Sprite)
	{
	}

	~SpriteComponent()
	{
	}

public:
	SDL_Texture* _texture;
};
