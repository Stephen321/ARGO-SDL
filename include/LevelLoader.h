#pragma once
#include <iostream>
#include "Factory.h"
#include "RenderSystem.h"
#include "ResourceIdentifier.h"
#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"

using namespace rapidjson;

class LevelLoader
{
public:
	LevelLoader() {};
	~LevelLoader() {};
	void LoadJson(const char* filename, std::vector<Entity*>& entities, RenderSystem& renderSystem, std::map<TextureID, SDL_Texture*>& _textureHolder);

private:
	
};

