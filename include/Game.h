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

	void							Initialize(SDL_Renderer* renderer) override;

	int								Update() override;
	void							Render() override;

	bool							IsRunning() override;

	void							Start() override;
	void							Stop() override;

	void							OnEvent(Event evt) override;

private:
	void							BindInput(Entity* player, Entity* weapon);

	void							LoadContent() override;
	void							CleanUp() override;

	SDL_Texture*					LoadTexture(const std::string & path) override;

	void							DebugBox2D();

private:
	EntityFactory					_entityFactory;
	BodyFactory						_bodyFactory;

	LevelLoader						_levelLoader;

	b2Vec2							_gravity;
	b2World							_world;

	std::vector<Entity*>			_entities;

	Graph							_waypoints;

	SystemManager					_systemManager;
	FunctionMaster					_functionMaster;
};

#endif

