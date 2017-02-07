#ifndef GAME_H
#define GAME_H

#include "Debug.h"

#include "SDL_image.h"

#include "FLInputManager.h"

#include "Entity.h"
#include "SpriteComponent.h"
#include "BoundsComponent.h"
#include "PhysicsComponent.h"

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "ControlSystem.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>

enum class TextureID
{
	Player,
};

class Game : public EventListener
{
public:
									Game();
									~Game();

	bool							Initialize(const char* title, int xpos, int ypos, int width, int height, int flags);
	
	void							Render();
	void							Update();
	void							LoadContent();
	void							CleanUp();

	void							OnEvent(Event evt);
	void							Test(int t);

	bool							IsRunning();

private:
	bool							SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags);

private:
	SDL_Window*						_window;
	SDL_Renderer*					_renderer;
	InputManager*					_inputManager = InputManager::GetInstance();

	std::map<TextureID, SDL_Texture*>_textureHolder;

	bool							_running;

	unsigned int					_lastTime;//time of last update;


	std::vector<Entity*>			_entities;
	RenderSystem					_renderSystem;
	PhysicsSystem					_physicSystem;
	ControlSystem*					_controlSystem;
	
};

class InputCommand : public Command
{
public:
	InputCommand(std::function<void()> function, EventListener::Type type) : Command(function, type) {}

	virtual void executePress()
	{
		for (int i = 0; m_type == EventListener::Type::Press && i < m_functions.size(); i++)
			m_functions[i]();
	}

	virtual void executeRelease()
	{
		for (int i = 0; m_type == EventListener::Type::Release && i < m_functions.size(); i++)
			m_functions[i]();
	}

	virtual void executeHold()
	{
		for (int i = 0; m_type == EventListener::Type::Hold && i < m_functions.size(); i++)
			m_functions[i]();
	}
};


#endif

