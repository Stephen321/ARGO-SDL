#include "LevelLoader.h"
#include "SpriteComponent.h"
#include "BoundsComponent.h"

#include <string>
void LevelLoader::LoadJson(const char* path, std::vector<Entity*>& entities, EntityFactory &entityFactory, BodyFactory &bodyFactory)
{
	FILE* fp = NULL;
	fopen_s(&fp, path, "rb");
	Document doc;
	if (fp != NULL)
	{
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		doc.ParseStream(is);
	}
	else
	{
		std::cout << "ERROR LOADING : " << path << std::endl;
		system("PAUSE");
		exit(0);
	}
	fclose(fp);

	/*
	Background layer
	*/

	int tileWidth = doc["tilewidth"].GetInt();
	int tileHeight = doc["tileheight"].GetInt();

	const Value& layerArray = doc["layers"];

	const Value& backgroundLayer = layerArray[0u];
	int backgroundLayerWidth = backgroundLayer["width"].GetInt();
	int backgroundLayerHeight = backgroundLayer["height"].GetInt();

	const Value& backgroundDataArray = backgroundLayer["data"];
	
	int index = 0;
	for (int y = 0; y < backgroundLayerHeight; y++)
	{
		for (int x = 0; x < backgroundLayerWidth; x++)
		{
			Entity* tile = entityFactory.CreateEntity(Entity::Type::Tile);
			SpriteComponent* sprite = static_cast<SpriteComponent*>(tile->GetComponent(Component::Type::Sprite));
			BoundsComponent* bounds = static_cast<BoundsComponent*>(tile->GetComponent(Component::Type::Bounds));

			switch (backgroundDataArray[index].GetInt())
			{
			case 1:
			{
				sprite->sourceRect = { 0 * tileWidth, 0, tileWidth, tileHeight };
				bounds->rect = SDL_Rect{ x * tileWidth, y * tileHeight, tileWidth, tileHeight };
				break;
			}
			case 2:
			{
				sprite->sourceRect = { 1 * tileWidth, 0, tileWidth, tileHeight };
				bounds->rect = SDL_Rect{ x * tileWidth, y * tileHeight, tileWidth, tileHeight };
				break;
			}
			case 3:
			{
				sprite->sourceRect = { 2 * tileWidth, 0, tileWidth, tileHeight };
				bounds->rect = SDL_Rect{ x * tileWidth, y * tileHeight, tileWidth, tileHeight };
				break;
			}
			default:
				break;
			}
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
		
		if (entityType == "colliderBox")
		{
			float x = entity["x"].GetFloat();
			float y = entity["y"].GetFloat();
			float w = entity["width"].GetFloat();
			float h = entity["height"].GetFloat();

			
		}
		else if(entityType == "colliderPoly")
		{
			const Value& entityPolyArray = entity["polygon"];
			int count = entityPolyArray.Size();
			b2Vec2* vectices = new b2Vec2[count];
			for (int i = 0; i < count; i++)
			{
				const Value& entityPoly = entityPolyArray[i];
				vectices[i].Set(entityPoly["x"].GetFloat(), entityPoly["y"].GetFloat());
			}
			
		}
		else if (entity == "checkpoint")
		{
			float x = entity["x"].GetFloat();
			float y = entity["y"].GetFloat();
			float w = entity["width"].GetFloat();
			float h = entity["height"].GetFloat();

		}
		else if (entity == "flag")
		{
			float x = entity["x"].GetFloat();
			float y = entity["y"].GetFloat();
			float w = entity["width"].GetFloat();
			float h = entity["height"].GetFloat();

		}
	}
}
