#pragma once

#include "FLInputManager.h"

#include "ResourceIdentifier.h"

class Scene : public EventListener
{
public:
	Scene();
	~Scene();

	virtual void		Initialize();

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