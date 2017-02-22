#pragma once

#include "Component.h"

#include "SDL_image.h"
#include "SDL_ttf.h"

#include <string>
#include <vector>


class UIComponent : public Component
{
public:
	UIComponent(SDL_Texture* texture)
		: texture(texture)
		, Component::Component(Component::Type::UI)
	{
		sourceRect = { 0, 0, 0, 0 };
		SDL_QueryTexture(texture, NULL, NULL, &sourceRect.w, &sourceRect.h);
	}

	UIComponent(SDL_Texture* texture, SDL_Rect source)
		: texture(texture), sourceRect(source)
		, Component::Component(Component::Type::UI)
	{
	}

	// Need more for text etc.

	~UIComponent()
	{
	}

public:
	SDL_Texture* texture;
	SDL_Rect sourceRect;

	TTF_Font*						_font = NULL;
	std::vector<SDL_Texture*>		_textTexture;
	std::vector<SDL_Rect>			_textRectangle;

	int								_fontSize;

	SDL_Renderer*					_renderer;

	std::vector<SDL_Rect>			_textRectangle;

};
