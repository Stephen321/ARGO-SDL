#include "SceneManager.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"
#include "NetworkHandler.h"
#include "Debug.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	DEBUG_MSG("Calling Cleanup");

	//_game->CleanUp();

	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

bool SceneManager::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	_running = SetupSDL(title, xpos, ypos, width, height, flags);

	if (_running)
	{
		_cameraSystem.Initialize(width, height);

		_menu = new MainMenu();
		_menu->Initialize(_renderer);
		_currentScene.push_back(_menu);

		//_game = new Game();
		//_game->Initialize(_renderer);
		//_currentScene.push_back(_game);

		//_lobby = new Lobby();
		//_lobby->Initialize(_renderer);
		//_currentScene.push_back(_lobby);

		//_options = new Options();
		//_options->Initialize(_renderer);
		//_currentScene.push_back(_options);

		//_about = new About();
		//_about->Initialize(_renderer);
		//_currentScene.push_back(_about);

		_runningScene = 0;
		_currentScene[_runningScene]->Start();
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
	if (_currentScene.back()->IsRunning())
	{
		_previousScene = _runningScene;
		_currentScene.back()->Render();
		_runningScene = _currentScene.back()->Update();

		// Stop Previous Scene
		if (_runningScene != _previousScene)
		{
			_currentScene.back()->Stop();

			// Delete Object

			_currentScene.pop_back();

			if (_runningScene == 0)
			{
				_menu = new MainMenu();
				_menu->Initialize(_renderer);
				_currentScene.push_back(_menu);
			}

			else if (_runningScene == 1)
			{
				_game = new Game();
				_game->Initialize(_renderer, _ids);
				_currentScene.push_back(_game);
			}

			else if (_runningScene == 2)
			{
				_lobby = new Lobby();
				_lobby->Initialize(_renderer, &_ids);
				_currentScene.push_back(_lobby);
			}

			else if (_runningScene == 3)
			{
				_options = new Options();
				_options->Initialize(_renderer);
				_currentScene.push_back(_options);
			}

			else if (_runningScene == 4)
			{
				_about = new About();
				_about->Initialize(_renderer);
				_currentScene.push_back(_about);
			}
		}
	}

	else
	{
		_running = false;
		if (NetworkHandler::Instance().GetConnected())
			NetworkHandler::Instance().Disconnect();
	}
}

bool SceneManager::IsRunning()
{
	return _running;
}
