#pragma once

#include <SDL.h>

#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Debug.h"

#include "Camera2D.h"
#include "FLInputManager.h"
#include "Scene.h"

#include "CameraSystem.h"

#include <vector>
#include <queue>
#include <map>

#include "ResourceIdentifier.h"
#include "FunctionMaster.h"
#include "RenderSystem.h"

class Options : public EventListener, public Scene
{
public:
	Options();
	~Options();

	void							Initialize(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);

	void							Render() override;
	int								Update() override;

	void							OnEvent(Event evt) override;

	bool							IsRunning() override;

private:
	void							BindInput();

	void							CreateText(string message, int x, int y);
	void							CreateTextColoured(string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a);

private:
	SDL_Window*						_window;
	SDL_Renderer*					_renderer;

	InputManager*					_inputManager = InputManager::GetInstance();

	std::map<TextureID, SDL_Texture*>_textureHolder;

	bool							_running;

	unsigned int					_lastTime;//time of last update;

	RenderSystem					_renderSystem;
	FunctionMaster					_functionMaster;
	CameraSystem					_cameraSystem;


	TTF_Font*						_font = NULL;
	std::vector<SDL_Texture*>		_textTexture;
	std::vector<SDL_Surface*>		_surface;
	std::vector<SDL_Rect>			_textRectangle;

	CurrentScene					_swapScene;
	int								_fontSize;
};

