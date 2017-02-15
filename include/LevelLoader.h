#pragma once
#include <iostream>
#include "RenderSystem.h"
#include "ResourceIdentifier.h"
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
#include "Box2D\Box2D.h"
#include "EntityFactory.h"
#include "BodyFactory.h"

using namespace rapidjson;

class LevelLoader
{
public:
	LevelLoader() {};
	~LevelLoader() {};
	void LoadJson(const char* path, std::vector<Entity*>& entities, EntityFactory* entityFactory, BodyFactory* bodyFactory);

private:
	
};

