#pragma once
#include <iostream>
#include "ResourceIdentifier.h"
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
#include "Box2D\Box2D.h"
#include "EntityFactory.h"
#include "BodyFactory.h"
#include "Graph.h"

using namespace rapidjson;

class LevelLoader
{
public:
	LevelLoader() {};
	~LevelLoader() {};

	void LoadJson(const char* path, std::vector<Entity*>& entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, Graph* waypoints);

private:
	void LoadTiles(const Value &tilesLayer, std::vector<Entity*>& entities, EntityFactory* ef, int tileWidth, int tileHeight);
	void LoadEntities(const Value &entitieLayer, std::vector<Entity*>& entities, EntityFactory* ef, BodyFactory* bf);
	void LoadWaypoints(const Value &waypointLayer, std::vector<Entity*>& entities, EntityFactory* ef, Graph* waypoints);
	void LoadColliders(const Value &colliderLayer, std::vector<Entity*>& entities, EntityFactory* ef, BodyFactory* bf);
	
};

