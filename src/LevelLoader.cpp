#include "LevelLoader.h"
#include <string>
void LevelLoader::LoadJson(const char* filename, std::vector<Entity*>& entities, RenderSystem& renderSystem, std::map<TextureID, SDL_Texture*>& _textureHolder)
{
	Factory factory = Factory();
	FILE* fp = NULL;
	fopen_s(&fp, filename, "rb");
	Document doc;
	if (fp != NULL)
	{
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		doc.ParseStream(is);
	}
	else
	{
		std::cout << "ERROR LOADING : " << filename << std::endl;
		system("PAUSE");
		exit(0);
	}
	fclose(fp);
	//
	int tileWidth = doc["tilewidth"].GetInt();
	int tileHeight = doc["tileheight"].GetInt();

	const Value& layerArray = doc["layers"];

	/*
	Background layer
	*/
	const Value& backgroundLayer = layerArray[0u];
	int backgroundLayerWidth = backgroundLayer["width"].GetInt();
	int backgroundLayerHeight = backgroundLayer["height"].GetInt();

	const Value& backgroundDataArray = backgroundLayer["data"];
	
	int index = 0;
	for (int y = 0; y < backgroundLayerHeight; y++)
	{
		for (int x = 0; x < backgroundLayerWidth; x++)
		{
			switch (backgroundDataArray[index].GetInt())
			{
			case 1:
			{
				entities.push_back(factory.CreateTile(_textureHolder[TextureID::TilemapSpriteSheet],
					SDL_Rect{ 0 * tileWidth, 0, tileWidth, tileHeight },
					SDL_Rect{ x * tileWidth, y * tileHeight, tileWidth, tileHeight }));
				
				break;
			}
			case 2:
			{
				entities.push_back(factory.CreateTile(_textureHolder[TextureID::TilemapSpriteSheet],
					SDL_Rect{ 1 * tileWidth, 0, tileWidth, tileHeight },
					SDL_Rect{ x * tileWidth, y * tileHeight, tileWidth, tileHeight }));
				break;
			}
			case 3:
			{
				entities.push_back(factory.CreateTile(_textureHolder[TextureID::TilemapSpriteSheet],
					SDL_Rect{ 2 * tileWidth, 0, tileWidth, tileHeight },
					SDL_Rect{ x * tileWidth, y * tileHeight, tileWidth, tileHeight }));
				break;
			}
			default:
				break;
			}
			renderSystem.AddEntity(entities.back());
			index++;
		}
	}

	/*
	Entity layer
	*/
	const Value& entitylayer = layerArray[1];

	const Value& entityDataArray = entitylayer["objects"];

	for (int i = 0; i < entityDataArray.Size(); i++)
	{
		const Value& entity = entityDataArray[i];
		string entityType = entity["name"].GetString();
		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();
		float w = entity["width"].GetFloat();
		float h = entity["height"].GetFloat();
		if (entityType == "checkpoint")
		{
			entities.push_back(factory.CreateWall(_textureHolder[TextureID::EntitySpriteSheet],
				SDL_Rect{ 1 * tileWidth, 0, tileWidth, tileHeight },
				SDL_Rect{ (int)x, (int)y , (int)w, (int)h }));
		}
		else if (entityType == "wall")
		{
			entities.push_back(factory.CreateWall(_textureHolder[TextureID::EntitySpriteSheet],
				SDL_Rect{ 2 * tileWidth, 0, tileWidth, tileHeight },
				SDL_Rect{ (int)x, (int)y , (int)w, (int)h }));
		}
		else if (entityType == "flag")
		{
			entities.push_back(factory.CreateWall(_textureHolder[TextureID::EntitySpriteSheet],
				SDL_Rect{ 0 * tileWidth, 0, tileWidth, tileHeight },
				SDL_Rect{ (int)x, (int)y , (int)w, (int)h }));
		}
		else
		{
			std::cout << "LOADING ERROR" << std::endl;
		}
		renderSystem.AddEntity(entities.back()); //for debugging
	}


	

	

}
