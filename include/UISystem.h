#pragma once

#include "System.h"

#include "SDL_image.h"
#include "SDL_ttf.h"

#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include "Camera2D.h"

class UISystem : public System
{
public:
	UISystem(float dt);
	~UISystem();

	void		Initialize(SDL_Renderer * renderer);

	void		Process(float dt = 0.f) override;

	void		CreateText(std::string message, int x, int y);
	void		CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	void		UpdateText(std::string message, int index);

private:
	SDL_Renderer*					_renderer;

	TTF_Font*						_font = NULL;
	std::vector<SDL_Texture*>		_textTexture;
	std::vector<SDL_Rect>			_textRectangle;

	int								_fontSize;
};

