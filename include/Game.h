#ifndef GAME_H
#define GAME_H

#include "SDL_image.h"
#include "Box2D\Box2D.h"

#include "Camera2D.h"
#include "FLInputManager.h"
#include "Scene.h"

#include "EntityFactory.h"
#include "BodyFactory.h"

#include "Entity.h"
#include "SystemManager.h"

#include <SDL.h>
#include <vector>
#include <queue>
#include <map>

#include "ResourceIdentifier.h"
#include "LevelLoader.h"
#include "Graph.h"
#include "FunctionMaster.h"




// Debug
using namespace Camera2D;

class Game : public EventListener, public Scene
{
public:
									Game();
									~Game();

	void							Initialize(SDL_Window*& window, SDL_Renderer*& renderer, int width, int height);
	
	void							Render() override;
	int								Update() override;

	void							LoadContent();
	void							CleanUp();

	void							OnEvent(Event evt) override;

	bool							IsRunning() override;
	SDL_Texture*					loadTexture(const std::string & path);

private:
	void							BindInput(Entity* player);

	void							DebugBox2D();

private:
	SDL_Window*						_window;
	SDL_Renderer*					_renderer;

	EntityFactory					_entityFactory;
	BodyFactory						_bodyFactory;

	LevelLoader						_levelLoader;

	InputManager*					_inputManager = InputManager::GetInstance();

	std::map<TextureID, SDL_Texture*>_textureHolder;

	b2Vec2							 _gravity;
	b2World							 _world;

	bool							_running;

	unsigned int					_lastTime;//time of last update;


	std::vector<Entity*>			_entities;

	Graph							_map;

	SystemManager					_systemManager;
	FunctionMaster					_functionMaster;

	CurrentScene					_swapScene;

};

#endif

