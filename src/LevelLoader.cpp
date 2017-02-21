#include "LevelLoader.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "CheckpointComponent.h"
#include "BasicTypes.h"
#include "Helpers.h"

void LevelLoader::LoadJson(const char* path, std::vector<Entity*>& entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, Graph* waypoints)
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

	const Value& layerArray = doc["layers"];
	LoadTiles(layerArray[0u], entities, entityFactory,  doc["tilewidth"].GetInt(), doc["tileheight"].GetInt());
	LoadEntities(layerArray[1], entities, entityFactory, bodyFactory);
	LoadWaypoints(layerArray[2], entities, entityFactory, waypoints);
	LoadColliders(layerArray[3], entities, entityFactory, bodyFactory);
}

void LevelLoader::LoadTiles(const Value &tileLayer, std::vector<Entity*>& entities, EntityFactory* ef, int tileWidth, int tileHeight)
{
	int mapWidth = tileLayer["width"].GetInt();
	int mapHeight = tileLayer["height"].GetInt();

	const Value& mapTileDataArray = tileLayer["data"];

	int index = 0;
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			Entity* tile = ef->CreateEntity(EntityType::Tile);
			SpriteComponent* sprite = static_cast<SpriteComponent*>(tile->GetComponent(Component::Type::Sprite));
			TransformComponent* bounds = static_cast<TransformComponent*>(tile->GetComponent(Component::Type::Transform));

			switch (mapTileDataArray[index].GetInt())
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

			entities.push_back(tile);
			index++;
		}
	}
}

void LevelLoader::LoadEntities(const Value &entitiesLayer, std::vector<Entity*>& entities, EntityFactory* ef, BodyFactory* bf)
{
	const Value& entityDataArray = entitiesLayer["objects"];

	int checkpointID = 0;
	bool createAi = false;
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

				Entity* player = ef->CreateEntity(EntityType::Player);
				ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));
				TransformComponent* transform = static_cast<TransformComponent*>(player->GetComponent(Component::Type::Transform));

				transform->rect.x = x;
				transform->rect.y = y;

				collider->body = bf->CreateBoxBody(
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
			else
			{

				Entity* ai = ef->CreateEntity(EntityType::AI);
				ColliderComponent* collider = static_cast<ColliderComponent*>(ai->GetComponent(Component::Type::Collider));
				TransformComponent* transform = static_cast<TransformComponent*>(ai->GetComponent(Component::Type::Transform));

				transform->rect.x = x;
				transform->rect.y = y;

				collider->body = bf->CreateBoxBody(
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
			createAi = true;
		}
		else if (entityName == "Checkpoint")
		{
			checkpointID++;
			Entity* checkpoint = ef->CreateEntity(EntityType::Checkpoint);

			ColliderComponent* collider = static_cast<ColliderComponent*>(checkpoint->GetComponent(Component::Type::Collider));
			TransformComponent* transform = static_cast<TransformComponent*>(checkpoint->GetComponent(Component::Type::Transform));

			CheckpointComponent* checkpointComponent = static_cast<CheckpointComponent*>(checkpoint->GetComponent(Component::Type::Checkpoint));
			checkpointComponent->id = checkpointID;

			transform->rect = { (int)(x + w*.5f), (int)(y + h*.5f), (int)(w), (int)(h) };
			transform->origin = { (int)(w*0.5f), (int)(h*0.5f) };

			b2Body* body = bf->CreateBoxBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x + w*.5f, y + h*.5f)
				, b2Vec2(w*.5f, h*.5f)
				, (uint16)checkpoint->GetType()
				, CHECKPOINT_MASK
				, true);

			body->SetUserData(checkpoint);
			entities.push_back(checkpoint);
		}
		else if (entityName == "Flag")
		{
			Entity* flag = ef->CreateEntity(EntityType::Flag);
			ColliderComponent* collider = static_cast<ColliderComponent*>(flag->GetComponent(Component::Type::Collider));
			TransformComponent* transform = static_cast<TransformComponent*>(flag->GetComponent(Component::Type::Transform));

			transform->rect = { (int)(x + w*.5f), (int)(y + h*.5f), (int)(w), (int)(h) };

			b2Body* body = bf->CreateBoxBody(
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
}

void LevelLoader::LoadWaypoints(const Value &waypointLayer, std::vector<Entity*>& entities, EntityFactory* ef, Graph* waypoints)
{
	//node
	const Value& wayPointArray = waypointLayer["objects"];
	int wayPointSize = wayPointArray.Size();

	for (int i = 0; i < wayPointSize; i++)
	{
		const Value& entity = wayPointArray[i];

		float x = entity["x"].GetFloat();
		float y = entity["y"].GetFloat();
		float w = entity["width"].GetFloat();
		float h = entity["height"].GetFloat();

		helper::Vector2 position = helper::Vector2(x + w*.5f, y + h*.5f);

		const Value& properties = entity["properties"];
		int currentNode = properties["node"].GetInt();

		waypoints->addNode(to_string(currentNode), position);

		Entity* point = ef->CreateEntity(EntityType::Point);
		TransformComponent* transform = static_cast<TransformComponent*>(point->GetComponent(Component::Type::Transform));
		transform->rect.x = x;
		transform->rect.y = y;
	}
	//arc
	for (int i = 0; i < wayPointSize; i++)
	{
		const Value& entity = wayPointArray[i];
		const Value& properties = entity["properties"];
		const Value& neighbour = properties["neighbour"];

		int fromNode = properties["node"].GetInt();

		for (int j = 0; j< neighbour.Size(); j++)
		{
			int toNode = neighbour[j].GetInt();
			int lenght = (waypoints->getNodes()[fromNode]->getPosition() - waypoints->getNodes()[toNode]->getPosition()).length();
			waypoints->addArc(fromNode, toNode, lenght, false);
		}
	}
	waypoints->addNode("flag", helper::Vector2(0, 0));
}

void LevelLoader::LoadColliders(const Value &colliderLayer, std::vector<Entity*>& entities, EntityFactory* ef, BodyFactory* bf)
{
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
			Entity* obstacle = ef->CreateEntity(EntityType::Obstacle);
			ColliderComponent* collider = static_cast<ColliderComponent*>(obstacle->GetComponent(Component::Type::Collider));

			b2Body* body = bf->CreateBoxBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x + w*.5f, y + h*.5f)
				, b2Vec2(w*.5f, h*.5f)
				, (uint16)obstacle->GetType()
				, OBSTACLE_MASK
				, false);//create body

			body->SetUserData(obstacle);
			entities.push_back(obstacle);
		}
		else if (entityName == "Poly")
		{
			Entity* obstacle = ef->CreateEntity(EntityType::Obstacle);
			ColliderComponent* collider = static_cast<ColliderComponent*>(obstacle->GetComponent(Component::Type::Collider));
			const Value& entityPolyArray = entity["polygon"]; //loop json entity
			int count = entityPolyArray.Size();

			b2Vec2* vertices = new b2Vec2[count];
			for (int i = 0; i < count; i++)
			{
				const Value& entityPoly = entityPolyArray[i];
				vertices[i].Set(pixelsToMeters(entityPoly["x"].GetFloat()), pixelsToMeters(entityPoly["y"].GetFloat()));
			}
			b2Body* body = bf->CreatePolyBody(
				b2BodyType::b2_staticBody
				, b2Vec2(x, y)
				, vertices
				, count
				, (uint16)obstacle->GetType()
				, OBSTACLE_MASK
				, false);

			body->SetUserData(obstacle);
			entities.push_back(obstacle);
		}
	}
}