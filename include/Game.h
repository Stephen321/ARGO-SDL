#ifndef GAME_H
#define GAME_H

#include "SDL_image.h"
#include "Box2D\Box2D.h"

#include "Camera2D.h"
#include "FLInputManager.h"

#include "EntityFactory.h"
#include "BodyFactory.h"

#include "Entity.h"

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "ControlSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "GunSystem.h"
#include "FiringSystem.h"

#include "WeaponSystem.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>

#include "ResourceIdentifier.h"
#include "LevelLoader.h"
#include "GraphNode.h"
#include "GraphArc.h"
#include "Graph.h"


using namespace Camera2D;

class Game : public EventListener
{
public:
									Game();
									~Game();

	void							Initialize(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);
	
	void							Render();
	void							Update();
	void							LoadContent();
	void							CleanUp();

	void							OnEvent(Event evt) override;

	bool							IsRunning(); 
	SDL_Texture*					loadTexture(const std::string & path);

private:
	void							BindInput(Entity* player);

	void							DebugBox2D();

private:
	SDL_Window*						_window;
	SDL_Renderer*					_renderer;

	EntityFactory*					_entityFactory;
	BodyFactory*					_bodyFactory;

	LevelLoader						_levelLoader;

	InputManager*					_inputManager = InputManager::GetInstance();

	std::map<TextureID, SDL_Texture*>_textureHolder;

	b2Vec2							 _gravity;
	b2World							 _world;

	bool							_running;

	unsigned int					_lastTime;//time of last update;


	std::vector<Entity*>*			_entities;

	RenderSystem					_renderSystem;
	PhysicsSystem					_physicsSystem;
	ControlSystem*					_controlSystem;
	CameraSystem					_cameraSystem;
	CollisionSystem					_collisionSystem;

	GunSystem						_gunSystem;
	FiringSystem*					_firingSystem;
	WeaponSystem					_weaponSystem;
	
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

	virtual void executeDown()
	{
		for (int i = 0; m_type == EventListener::Type::Down && i < m_functions.size(); i++)
			m_functions[i]();
	}
};


#endif

