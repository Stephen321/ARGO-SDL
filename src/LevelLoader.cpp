#include "LevelLoader.h"
#include "SpriteComponent.h"
#include "Helpers.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

void LevelLoader::LoadJson(const char* path, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory)
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
			Entity* tile = entityFactory->CreateEntity(Entity::Type::Tile);
			SpriteComponent* sprite = static_cast<SpriteComponent*>(tile->GetComponent(Component::Type::Sprite));
			TransformComponent* bounds = static_cast<TransformComponent*>(tile->GetComponent(Component::Type::Transform));

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

	
	//Entities
	const Value& entitylayer = layerArray[1];
	const Value& entityDataArray = entitylayer["objects"];
	bool createPlayer = false;
	for (int i = 0; i < entityDataArray.Size(); i++)
	{
		const Value& entity = entityDataArray[i];
		string entityName = entity["name"].GetString();

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();
		float w = entity["width"].GetFloat();
		float h = entity["height"].GetFloat();
		if (entityName == "PlayerSolo")
		{
			if (!createPlayer)
			{
				createPlayer = true;
				Entity* player = entityFactory->CreateEntity(Entity::Type::Player);
				ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));
				SpriteComponent* spriteComponent = static_cast<SpriteComponent*>(player->GetComponent(Component::Type::Sprite));
				collider->body = bodyFactory->CreateBoxBody(b2BodyType::b2_dynamicBody, b2Vec2(x + w*.5f, y + h*.5f), 0.f, b2Vec2(spriteComponent->sourceRect.w / 2, spriteComponent->sourceRect.h / 2), false);
				collider->body->SetUserData(player);
				collider->body->SetFixedRotation(true);
				entities->push_back(player);
				
			}
		}
		else if (entityName == "Checkpoint")
		{
			

			b2Body* body = bodyFactory->CreateBoxBody(b2BodyType::b2_staticBody, b2Vec2(x +w*.5f, y+h*.5f), 0, b2Vec2(w*.5f, h*.5f), true);

		}
		else if (entityName == "Flag")
		{
			//idk type :P
			b2Body* body = bodyFactory->CreateBoxBody(b2BodyType::b2_dynamicBody, b2Vec2(x + w*.5f, y + h*.5f), 0, b2Vec2(w*.5f, h*.5f), true);
		}

	}
	
	//Waypoints
	const Value& wayPointLayer = layerArray[2];
	const Value& wapPointArray = wayPointLayer["objects"];

	for (int i = 0; i < wapPointArray.Size(); i++)
	{
		const Value& entity = wapPointArray[i];
		string entityName = entity["name"].GetString();

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();

	}


	//Colliders
	const Value& colliderLayer = layerArray[3];
	const Value& colliderDataArray = colliderLayer["objects"];

	for (int i = 0; i < colliderDataArray.Size(); i++)
	{
		const Value& entity = colliderDataArray[i];
		string entityName = entity["name"].GetString();

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();

		if (entityName == "Box")
		{
			float w = entity["width"].GetFloat();
			float h = entity["height"].GetFloat();
			b2Body* body = bodyFactory->CreateBoxBody(b2BodyType::b2_staticBody, b2Vec2(x +w*.5f, y+h*.5f), 0, b2Vec2(w*.5f, h*.5f), false);	//create body
		}
		else if (entityName == "Poly")
		{
			
				const Value& entityPolyArray = entity["polygon"]; //loop json entity
				int count = entityPolyArray.Size();

				b2Vec2* vertices = new b2Vec2[count];
				for (int i = 0; i < count; i++)
				{
					const Value& entityPoly = entityPolyArray[i];
					vertices[i].Set(pixelsToMeters(entityPoly["x"].GetFloat()), pixelsToMeters(entityPoly["y"].GetFloat()));
				}
				b2Body* body = bodyFactory->CreatePolyBody(b2BodyType::b2_staticBody, b2Vec2(x, y), 0, vertices, count, false);	//create body
			

		}
	}
}
