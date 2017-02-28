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

	void							Initialize(SDL_Renderer * renderer);

	void							Process(float dt = 0.f) override;


	void							DeleteText();
	void							DeleteDisplayText();

	void							CreateText(std::string message, int x, int y);
	void							CreateTextAtOrigin(std::string message, int x, int y);
	void							CreateTextAtCenter(std::string message, int x, int y);
	void							CreateTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	void							UpdateText(std::string message, int index);
	void							UpdateTextAtCenter(std::string message, int index);
	void							UpdateTextColoured(std::string message, int index, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

	void							CreateDisplayText(std::string message, int x, int y);
	int								CreateDisplayTextColoured(std::string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a);
	void							DeleteDisplayTextByID(int id);

	std::vector<SDL_Rect>&	GetInteractiveTextRectangle();
	std::vector<SDL_Rect>&	GetDisplayTextRectangle();

private:
	SDL_Renderer*					_renderer;

	TTF_Font*						_font = NULL;
	int								_fontSize;

	std::vector<SDL_Rect>			_displayTextRectangle;
	std::vector<SDL_Rect>			_interactiveTextRectangle;

	std::vector<SDL_Texture*>		_displayTextTexture;
	std::vector<SDL_Texture*>		_interactiveTextTexture;

};

