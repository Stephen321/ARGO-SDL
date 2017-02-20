#include "SceneManager.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

#include "Debug.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	DEBUG_MSG("Calling Cleanup");
	_game->CleanUp();
}

bool SceneManager::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	_running = SetupSDL(title, xpos, ypos, width, height, flags);

	if (_running)
	{
		_cameraSystem.Initialize(width, height);

		_menu = new MainMenu();
		_menu->Initialize(_window, _renderer, width, height);

		_game = new Game();
		_game->Initialize(_window, _renderer, width, height);

		_lobby = new Lobby();
		_lobby->Initialize(_window, _renderer, width, height);

		_options = new Options();
		_options->Initialize(_window, _renderer, width, height);

		_about = new About();
		_about->Initialize(_window, _renderer, width, height);

		_currentScene.push_back(_menu);
		_currentScene.push_back(_game);
		_currentScene.push_back(_lobby);
		_currentScene.push_back(_options);
		_currentScene.push_back(_about);

		_runningScene = 0;
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
				if (SDLNet_Init() != 0)
				{
					DEBUG_MSG("SDLNet init success");
				}
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
	if (_currentScene[_runningScene]->IsRunning())
	{
		_runningScene = _currentScene[_runningScene]->Update();
		_currentScene[_runningScene]->Render();
	}

	else
	{
		_running = false;
	}
}

bool SceneManager::IsRunning()
{
	return _running;
}
