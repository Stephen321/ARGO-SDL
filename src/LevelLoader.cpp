#include "LevelLoader.h"
#include "SpriteComponent.h"
#include "Helpers.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "BasicTypes.h"

void LevelLoader::LoadJson(const char* path, std::vector<Entity*>& entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, Graph* _map)
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
			Entity* tile = entityFactory->CreateEntity(EntityType::Tile);
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

			entities.push_back(tile);
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
			{ //it would be really nice if we could move all of this body creation stuff into the entity factory some how
				createPlayer = true;

				Entity* player = entityFactory->CreateEntity(EntityType::Player);
				ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));
				TransformComponent* transform = static_cast<TransformComponent*>(player->GetComponent(Component::Type::Transform));

				transform->rect.x = x;
				transform->rect.y = y;

				collider->body = bodyFactory->CreateBoxBody(
					  b2BodyType::b2_dynamicBody
					, b2Vec2(transform->rect.x - transform->origin.x * transform->scaleX, transform->rect.y - transform->origin.x * transform->scaleY)
					, b2Vec2(transform->rect.w / 2, transform->rect.h / 2)
					, (uint16)player->GetType()
					, PLAYER_MASK
					, false);

				collider->body->SetUserData(player);
				collider->body->SetFixedRotation(true);
				entities.push_back(player);
				
			}
		}
		else if (entityName == "Checkpoint")
		{
			Entity* checkpoint = entityFactory->CreateEntity(EntityType::Checkpoint);
			ColliderComponent* collider = static_cast<ColliderComponent*>(checkpoint->GetComponent(Component::Type::Collider));

			b2Body* body = bodyFactory->CreateBoxBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x +w*.5f, y+h*.5f)
				, b2Vec2(w*.5f, h*.5f)
				, (uint16)checkpoint->GetType()
				, CHECKPOINT_MASK
				, true);

			body->SetUserData(checkpoint);
			entities.push_back(checkpoint);
		}
		else if (entityName == "Flag")
		{
			Entity* flag = entityFactory->CreateEntity(EntityType::Flag);
			ColliderComponent* collider = static_cast<ColliderComponent*>(flag->GetComponent(Component::Type::Collider));

			b2Body* body = bodyFactory->CreateBoxBody(
				b2BodyType::b2_dynamicBody
				, b2Vec2(x + w*.5f, y + h*.5f)
				, b2Vec2(w*.5f, h*.5f)
				, (uint16)flag->GetType()
				, FLAG_MASK
				, true);

			body->SetUserData(flag);
			entities.push_back(flag);
		}

	}
	
	//Waypoints
	const Value& wayPointLayer = layerArray[2];
	const Value& wayPointArray = wayPointLayer["objects"];
	//adding node
	int wayPointSize = wayPointArray.Size();

	for (int i = 0; i < wayPointSize; i++)
	{
		const Value& entity = wayPointArray[i];

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();
		float w = entity["width"].GetFloat();
		float h = entity["height"].GetFloat();
		
		helper::Vector2 position = helper::Vector2(x + w*.5f, y+ h*.5f);

		const Value& properties = entity["properties"];
		int currentNode = properties["node"].GetInt();

		_map->addNode(to_string(currentNode) , position);

		Entity* point = entityFactory->CreateEntity(EntityType::Point);

		b2Body* body = bodyFactory->CreateBoxBody(
			b2BodyType::b2_staticBody
			, b2Vec2(x + w*.5f, y + h*.5f)
			, b2Vec2(w*.5f, h*.5f)
			, (uint16)point->GetType()
			, 0
			, true);

		body->SetUserData(point);

		TransformComponent* transform = static_cast<TransformComponent*>(point->GetComponent(Component::Type::Transform));
		transform->rect.x = x;
		transform->rect.y = y;
		//entities->push_back(point); instead push back to pathfinder?

		if (currentNode == 0)
		{
			Entity* ai = entityFactory->CreateEntity(EntityType::AI);
			ColliderComponent* collider = static_cast<ColliderComponent*>(ai->GetComponent(Component::Type::Collider));
			TransformComponent* transform = static_cast<TransformComponent*>(ai->GetComponent(Component::Type::Transform));

			transform->rect.x = x;
			transform->rect.y = y;

			collider->body = bodyFactory->CreateBoxBody(
				b2BodyType::b2_dynamicBody
				, b2Vec2(transform->rect.x - transform->origin.x * transform->scaleX, transform->rect.y - transform->origin.x * transform->scaleY)
				, b2Vec2(transform->rect.w / 2, transform->rect.h / 2)
				, (uint16)ai->GetType()
				, AI_MASK
				, false);

			collider->body->SetUserData(ai);
			collider->body->SetFixedRotation(true);
			entities.push_back(ai);
		

		}
	}
	//adding arch
	for (int i = 0; i < wayPointSize; i++)
	{
		const Value& entity = wayPointArray[i];
		const Value& properties = entity["properties"];
		const Value& neighbour = properties["neighbour"];
		
		int fromNode = properties["node"].GetInt();
		
		for (int j = 0; j< neighbour.Size(); j++)
		{
			int toNode = neighbour[j].GetInt();
			int lenght = (_map->nodeArray()[fromNode]->getPosition() - _map->nodeArray()[toNode]->getPosition()).length();
			_map->addArc(fromNode, toNode, lenght, false);
		}
		_map->removeNode(3);
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
			Entity* obstacle = entityFactory->CreateEntity(EntityType::Obstacle);
			ColliderComponent* collider = static_cast<ColliderComponent*>(obstacle->GetComponent(Component::Type::Collider));

			b2Body* body = bodyFactory->CreateBoxBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x +w*.5f, y+h*.5f)
				, b2Vec2(w*.5f, h*.5f)
				, (uint16)obstacle->GetType()
				, OBSTACLE_MASK
				, false);//create body

			body->SetUserData(obstacle);
			entities.push_back(obstacle);
		}
		else if (entityName == "Poly")
		{
			Entity* obstacle = entityFactory->CreateEntity(EntityType::Obstacle);
			ColliderComponent* collider = static_cast<ColliderComponent*>(obstacle->GetComponent(Component::Type::Collider));
			const Value& entityPolyArray = entity["polygon"]; //loop json entity
			int count = entityPolyArray.Size();

			b2Vec2* vertices = new b2Vec2[count];
			for (int i = 0; i < count; i++)
			{
				const Value& entityPoly = entityPolyArray[i];
				vertices[i].Set(pixelsToMeters(entityPoly["x"].GetFloat()), pixelsToMeters(entityPoly["y"].GetFloat()));
			}
			b2Body* body = bodyFactory->CreatePolyBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x, y)
				, vertices
				, count
				, (uint16)obstacle->GetType()
				, OBSTACLE_MASK
				, false);	//create body

			body->SetUserData(obstacle);
			entities.push_back(obstacle);
		}
	}
	
	
}
