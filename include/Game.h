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

#include <SDL.h>#
#include <vector>
#include <queue>
#include <map>

#include "ResourceIdentifier.h"
#include "LevelLoader.h"
#include "Graph.h"
#include "FunctionMaster.h"




// Debug
using namespace Camera2D;

class Game : public Scene
{
public:
									Game();
									~Game();

	void							Initialize(SDL_Renderer* renderer, const std::vector<int>& ids = std::vector<int>());

	int								Update() override;
	void							Render() override;

	bool							IsRunning() override;

	void							Start() override;
	void							Stop() override;

	void							OnEvent(Event evt) override;

private:
	void							BindInput();

	void							LoadContent(const std::vector<int>& ids);
	void							CleanUp() override;

	SDL_Texture*					LoadTexture(const std::string & path) override;

	void							DebugBox2D();

	void							CreateUI();
	void							UpdateUI();

private:
	EntityFactory					_entityFactory;
	BodyFactory						_bodyFactory;

	LevelLoader						_levelLoader;

	InputManager*					_inputManager = InputManager::GetInstance();

	std::map<TextureID, SDL_Texture*>_textureHolder;

	b2Vec2							 _gravity;
	b2World							 _world;

	bool							_running;

	unsigned int					_lastTime;//time of last update;

	Graph							_waypoints;

	SystemManager					_systemManager;
	FunctionMaster					_functionMaster;

	Entity*							player;
};

#endif

