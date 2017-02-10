#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "Game.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"



Game::Game() 
	: _running(false)
	, _textureHolder(std::map<TextureID, SDL_Texture*>())
	, _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _collisionSystem(COLLISION_SYSTEM_UPDATE)
	, _renderSystem(_renderer, &_cameraSystem.getCamera())
	, _physicsSystem()
	, _controlSystem(new ControlSystem(&_cameraSystem.getCamera(), 0.0f))
	, _gravity(0.f, 0.f)
	, _world(_gravity)
	, _entityFactory(nullptr)
	, _bodyFactory(nullptr)
{
	_world.SetContactListener(&_collisionSystem);
	_world.SetAllowSleeping(false);
	_entityFactory = new EntityFactory(&_renderSystem, &_physicsSystem, _controlSystem, &_cameraSystem, &_textureHolder);
	_bodyFactory = new BodyFactory(&_world);
}

Game::~Game()
{
	_world.~b2World();
}

bool Game::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{

	_running = SetupSDL(title, xpos, ypos, width, height, flags);
	
	_cameraSystem.Init(width, height);
	if (_running)
	{//SETUP WHATEVER NEEDS TO BE 

		LoadContent();

		Entity* player = nullptr;
		std::vector<Entity*>::iterator it = _entities.begin();
		while (it != _entities.end())
		{
			if ((*it)->GetType() == Entity::Type::Player)
			{
				player = *it;
				break;
			}
			it++;
		}


		
		Entity* weapon = new Entity(Entity::Type::Weapon);

		SpriteComponent* spriteComponent = new SpriteComponent(_textureHolder[TextureID::Weapon]);

		weapon->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.25f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
		weapon->AddComponent(spriteComponent);

		_entities.push_back(weapon);

		_renderSystem.AddEntity(_entities.back());
		_controlSystem->AddTurret(_entities.back());
		
		_weaponSystem.AddEntity(player, weapon);


		// Input
		Command* wIn = new InputCommand(std::bind(&ControlSystem::MovePlayer, _controlSystem, 0, -1, player), Type::Down);
		_inputManager->AddKey(Event::w, wIn, this);

		Command* aIn = new InputCommand(std::bind(&ControlSystem::MovePlayer, _controlSystem, -1, 0, player), Type::Down);
		_inputManager->AddKey(Event::a, aIn, this);

		Command* sIn = new InputCommand(std::bind(&ControlSystem::MovePlayer, _controlSystem, 0, 1, player), Type::Down);
		_inputManager->AddKey(Event::s, sIn, this);

		Command* dIn = new InputCommand(std::bind(&ControlSystem::MovePlayer, _controlSystem, 1, 0, player), Type::Down);
		_inputManager->AddKey(Event::d, dIn, this);

		_inputManager->AddListener(Event::ESCAPE, this);
	}

	return _running;
}
bool Game::SetupSDL(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		DEBUG_MSG("SDL Init success");
		_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (_window != 0)
		{
			DEBUG_MSG("Window creation success");
			_renderer = SDL_CreateRenderer(_window, -1, 0);
			if (_renderer != 0)
			{
				DEBUG_MSG("Renderer creation success");
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			}
			else
			{
				DEBUG_MSG("Renderer init fail");
				return false;
			}
		}
		else
		{
			DEBUG_MSG("Window init fail");
			return false;
		}
	}
	else
	{
		DEBUG_MSG("SDL init fail");
		return false;
	}

	return true;
}

void Game::LoadContent()
{
	_textureHolder[TextureID::TilemapSpriteSheet] = loadTexture("Media/Textures/BackgroundSprite.png");

	_textureHolder[TextureID::Weapon] = loadTexture("Media/Player/Weapon.png");
	_textureHolder[TextureID::Player] = loadTexture("Media/Player/player.png");

	_textureHolder[TextureID::EntitySpriteSheet] = loadTexture("Media/Textures/EntitySprite.png");

	_levelLoader.LoadJson("Media/Json/Map.json",&_entities,_entityFactory, _bodyFactory);
	 //_levelLoader.LoadJson("Media/Json/Map2.json", _entities, _renderSystem, _textureHolder);

}

void Game::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update

	//UPDATE HERE

	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();
	// Use to Update constantly at frame rate
	_inputManager->ConstantInput();

	_cameraSystem.Process(dt);
	_physicsSystem.Process(dt);
	_collisionSystem.Process(dt);
	_weaponSystem.Process(dt);

	_controlSystem->Process(dt);

	_world.Step(1 / (float)SCREEN_FPS, 8, 3);
	//save the curent time for next frame
	_lastTime = currentTime;
}

void Game::Render()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
	
	//test background in order to see the camera is following the player position

	//RENDER HERE
	_renderSystem.Process();

	//test draw world bounds
	SDL_Rect r = { 0, 0, WORLD_WIDTH, WORLD_HEIGHT };
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(_renderer, &_cameraSystem.getCamera().worldToScreen(r));

//DEBUG
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	///////////////////use this code for testing purpose///////////////////////////////////////////////////////////////
	for (b2Body* BodyIterator = _world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{

				b2Shape::Type shapeType = b2Fixture->GetType();
				if (shapeType == b2Shape::e_circle)
				{
				}
				else if (shapeType == b2Shape::e_polygon)
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();

					int lenght = (int)polygonShape->GetVertexCount();

					SDL_Point* points = new SDL_Point[lenght + 1];


					for (int i = 0; i < lenght; i++)
					{
						Point worldPoint;
						float verticesPosX = polygonShape->GetVertex(i).x; b2Fixture->GetBody()->GetPosition().x;
						float verticesPosY = polygonShape->GetVertex(i).y; b2Fixture->GetBody()->GetPosition().y;
						/*
						float mag = sqrt(fixturePosX * fixturePosX + fixturePosY * fixturePosY);
						if (mag != 0)
						{
						fixturePosX /= mag;
						fixturePosY /= mag;
						}*/
						float angle = b2Fixture->GetBody()->GetAngle();
						float s = sin(angle);
						float c = cos(angle);

						// translate point back to origin:
						verticesPosX -= 0;
						verticesPosY -= 0;

						// rotate point
						float xnew = verticesPosX* c - verticesPosY * s;
						float ynew = verticesPosX * s + verticesPosY * c;

						// translate point back:
						verticesPosX = xnew + 0;
						verticesPosY = ynew + 0;

						worldPoint.x = metersToPixels(verticesPosX + b2Fixture->GetBody()->GetPosition().x);
						worldPoint.y = metersToPixels(verticesPosY + b2Fixture->GetBody()->GetPosition().y);
						worldPoint = _cameraSystem.getCamera().worldToScreen(worldPoint);
						points[i].x = worldPoint.x;
						points[i].y = worldPoint.y;
					}

					points[lenght].y = points[0].y;
					points[lenght].x = points[0].x;



					SDL_RenderDrawLines(_renderer, points, lenght + 1);
				}
			}
		}
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

bool Game::IsRunning()
{
	return _running;
}


void Game::CleanUp()
{
	DEBUG_MSG("Cleaning Up");

	//DESTROY HERE
	_world.SetAllowSleeping(true);

	for (int i = 0; i < _entities.size(); i++)
	{
		delete _entities[i];
	}
	_entities.clear();

	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void Game::OnEvent(EventListener::Event evt)
{
	switch (evt)
	{
		case Event::ESCAPE:
			_inputManager->saveFile();
			_running = false;
	}
}

void Game::Test(int t)
{
	int i = t;
}

SDL_Texture * Game::loadTexture(const std::string & path)
{
	SDL_Texture* texture = NULL;

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(_renderer, surface);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}