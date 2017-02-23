#pragma once


// C++
#include <vector>
#include <queue>
#include <map>

// SDL
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

// Box2D
#include "Debug.h"

// Components
#include "FLInputManager.h"
#include "Camera2D.h"

// Resources
#include "ResourceIdentifier.h"
#include "LevelLoader.h"

// Systems
#include "FunctionMaster.h"

// Helpers
#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

class Scene : public EventListener
{
public:
	Scene();
	~Scene();

	virtual void		Initialize(SDL_Renderer* renderer);

	virtual int			Update();
	virtual void		Render();

	virtual bool		IsRunning();

	virtual void		Start();
	virtual void		Stop();

protected:

	enum class CurrentScene {
		MAIN_MENU,
		GAME,
		LOBBY,
		OPTIONS,
		ABOUT
	};

	virtual void						BindInput();

	virtual void						LoadContent();
	virtual void						CleanUp();

	virtual SDL_Texture*				LoadTexture(const std::string & path);

	InputManager*						_inputManager = InputManager::GetInstance();

	SDL_Renderer*						_renderer;

	std::map<TextureID, SDL_Texture*>	_textureHolder;

	bool								_running;
	unsigned int						_lastTime; //time of last update;

	CurrentScene						_swapScene;
};