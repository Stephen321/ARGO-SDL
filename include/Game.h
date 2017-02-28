#ifndef GAME_H
#define GAME_H


#include "Box2D\Box2D.h"
#include "Camera2D.h"
#include "Scene.h"

#include "EntityFactory.h"
#include "BodyFactory.h"

#include "Entity.h"
#include "SystemManager.h"

#include "LevelLoader.h"
#include "Graph.h"
#include "FunctionMaster.h"


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

	void							DebugBox2D();

private:
	EntityFactory					_entityFactory;
	BodyFactory						_bodyFactory;

	LevelLoader						_levelLoader;

	b2Vec2							 _gravity;
	b2World							 _world;

	Graph							_waypoints;

	SystemManager					_systemManager;
	FunctionMaster					_functionMaster;

	Entity*							_player;
	InputManager*					_inputManager = InputManager::GetInstance();

	std::map<TextureID, SDL_Texture*>_textureHolder;

	bool							_running;
	unsigned int					_lastTime;//time of last update;
};

#endif

