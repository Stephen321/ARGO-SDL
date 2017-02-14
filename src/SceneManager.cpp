#include "SceneManager.h"
#include "Debug.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	DEBUG_MSG("Calling Cleanup");
	game->CleanUp();
}

bool SceneManager::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	_running = SetupSDL(title, xpos, ypos, width, height, flags);

	if (_running)
	{
		_cameraSystem.Initialize(width, height);

		game = new Game();
		game->Initialize(_window, _renderer, width, height);
	}

	return _running;
}

bool SceneManager::SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		DEBUG_MSG("SDL Init success");
		_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (_window != 0)
		{
			DEBUG_MSG("Window creation success");
			_renderer = SDL_CreateRenderer(_window, -1, 0);
			if (_renderer != 0)
			{
				DEBUG_MSG("Renderer creation success");
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			}
			else
			{
				DEBUG_MSG("Renderer init fail");
				return false;
			}
		}
		else
		{
			DEBUG_MSG("Window init fail");
			return false;
		}
	}
	else
	{
		DEBUG_MSG("SDL init fail");
		return false;
	}

	return true;
}

void SceneManager::Update()
{
	if (game->IsRunning())
	{
		game->Update();
		game->Render();
	}


	if(!game->IsRunning())
	{
		_running = false;
	}
}

bool SceneManager::IsRunning()
{
	return _running;
}
