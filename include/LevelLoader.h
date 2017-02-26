#pragma once
#include <iostream>
#include "ResourceIdentifier.h"
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
#include "Box2D\Box2D.h"
#include "EntityFactory.h"
#include "BodyFactory.h"
#include "Graph.h"
#include "SystemManager.h"

using namespace rapidjson;

class LevelLoader
{
public:
	LevelLoader() {};
	~LevelLoader() {};

	void LoadJson(const char* path, SystemManager& SystemManager, BodyFactory* bodyFactory, Graph* waypoints);

private:
	void LoadTiles(const Value &tilesLayer, SystemManager& SystemManager, int tileWidth, int tileHeight);
	void LoadEntities(const Value &entitieLayer, SystemManager& SystemManager);
	void LoadWaypoints(const Value &waypointLayer, SystemManager& SystemManager, Graph* waypoints);
	void LoadColliders(const Value &colliderLayer, SystemManager& SystemManager, BodyFactory* bodyFactory);
	
};

