#pragma once

#include <SDL.h>

#include "Camera2D.h"

#include "MainMenu.h"
#include "Game.h"

#include "CameraSystem.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool							Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	void							Update();

	bool							IsRunning();

private:
	bool							SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);

private:
	SDL_Window*						_window;
	SDL_Renderer*					_renderer;

	bool							_running;

	CameraSystem					_cameraSystem;

private:
	MainMenu* menu;
	Game* game;
};

