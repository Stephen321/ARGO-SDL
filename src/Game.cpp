#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "Game.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

#include <assert.h>
#include "GunComponent.h"
#include "DestructionComponent.h"
#include "FlagComponent.h"

Game::Game() 
	: _gravity(0.f, 0.f)
	, _world(_gravity)
	, _waypoints()
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Game::~Game()
{
	_world.~b2World();
}

void Game::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;

	_systemManager.Initialize(_renderer, &_entityFactory, &_bodyFactory, &_world, SCREEN_WIDTH, SCREEN_HEIGHT);


	_world.SetAllowSleeping(false);

	_entityFactory.Initialize(&_textureHolder);
	_bodyFactory.Initialize(&_world);

	Start();
	LoadContent();

	Entity* player = nullptr;

	_systemManager.PostInitialize(player, &_waypoints);


	//shooting
	//Command* spaceIn = new InputCommand(std::bind(&FunctionMaster::FireBullet, _functionMaster, weapon), Type::Press);
	//_inputManager->AddKey(Event::SPACE, spaceIn, this);

	_swapScene = CurrentScene::GAME;
	BindInput(player);
}

void Game::LoadContent()
{
	_textureHolder[TextureID::TilemapSpriteSheet] = LoadTexture("Media/Textures/SpriteSheetFull.png");

	_textureHolder[TextureID::Bullet] = LoadTexture("Media/Player/Bullet.png");
	_textureHolder[TextureID::Weapon] = LoadTexture("Media/Player/Weapon.png");
	_textureHolder[TextureID::Flag] = LoadTexture("Media/Player/Flag.png");
	_textureHolder[TextureID::Player] = LoadTexture("Media/Player/player.png");
	_textureHolder[TextureID::Checkpoint] = LoadTexture("Media/Textures/Checkpoint.png");

	_textureHolder[TextureID::EntitySpriteSheet] = LoadTexture("Media/Textures/EntitySprite.png");
	_levelLoader.LoadJson("Media/Json/NormalMap.json", _systemManager, &_bodyFactory, &_waypoints);
}


int Game::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update

	// UPDATE HERE //
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();
	// Use to Update constantly at frame rate
	_inputManager->ConstantInput();

	_systemManager.Process(dt);

	_world.Step(1 / (float)SCREEN_FPS, 8, 3);
	//save the curent time for next frame
	_lastTime = currentTime;

	return (int)_swapScene;
}

void Game::Render()
{
	SDL_RenderClear(_renderer);	

	//RENDER HERE
	_systemManager.Render();
	DebugBox2D();

	SDL_RenderPresent(_renderer);
}

bool Game::IsRunning()
{
	if (_swapScene != CurrentScene::GAME) { _swapScene = CurrentScene::GAME; }
	return _running;
}

void Game::Start()
{
	_running = true;
	_swapScene = CurrentScene::GAME;
}

void Game::Stop()
{
	_running = false;
	CleanUp();
	_inputManager->EmptyKeys();
}

void Game::OnEvent(EventListener::Event evt)
{
	/*
	if (_running)
	{
		switch (evt)
		{
		case Event::ESCAPE:
			//_inputManager->saveFile();
			_running = false;

		case Event::w:
			_audioManager->PlayFX("Hum");
		case Event::a:
			_audioManager->PlayFX("Hum");
		case Event::s:
			_audioManager->PlayFX("Hum");
		case Event::d:
			_audioManager->PlayFX("Hum");
		}
	}*/
}

void Game::BindInput(Entity* player)
{
	// Delete Key binding
	Command* nIn = new InputCommand([&]()
	{
		_inputManager->EmptyKey(Event::BACKSPACE);
	}, Type::Press);

	_inputManager->AddKey(Event::NUM_0, nIn, this);


	Command* wIn = new InputCommand(std::bind(&FunctionMaster::MoveVertical, &_functionMaster, -1, player), Type::Down);
	_inputManager->AddKey(Event::w, wIn, this);

	Command* aIn = new InputCommand(std::bind(&FunctionMaster::MoveHorizontal, &_functionMaster, -1, player), Type::Down);
	_inputManager->AddKey(Event::a, aIn, this);

	Command* sIn = new InputCommand(std::bind(&FunctionMaster::MoveVertical, &_functionMaster, 1, player), Type::Down);
	_inputManager->AddKey(Event::s, sIn, this);

	Command* dIn = new InputCommand(std::bind(&FunctionMaster::MoveHorizontal, &_functionMaster, 1, player), Type::Down);
	_inputManager->AddKey(Event::d, dIn, this);

	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);


	// Recreate key binding
	Command* noIn = new InputCommand([&]()
	{
		Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
		_inputManager->AddKey(Event::BACKSPACE, backIn, this);
	}, Type::Press);

	_inputManager->AddKey(Event::NUM_1, noIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);

}

void Game::CleanUp()
{
	//DESTROY HERE
	_world.SetAllowSleeping(true);
	_world.~b2World();

	_systemManager.~SystemManager();

	//SDL_DestroyWindow(_window);
	//SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}


SDL_Texture* Game::LoadTexture(const std::string & path)
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

void Game::DebugBox2D()
{
	//DEBUG ASTAR
	
	//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_waypoints.drawNodes(_renderer, &_systemManager.GetCamera());

	_waypoints.drawArcs(_renderer, &_systemManager.GetCamera());
	//DEBUG BOX2D
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

	///////////////////use this code for testing purpose///////////////////////////////////////////////////////////////
	for (b2Body* BodyIterator = _world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			if (BodyIterator->GetUserData())
			{
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
			}
			else
			{
				Entity* e = static_cast<Entity*>(BodyIterator->GetUserData());
				EntityType t = e->GetType();
				if (t == EntityType::Player)
				{
					SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				}
				else if (t == EntityType::Checkpoint)
				{
					SDL_SetRenderDrawColor(_renderer, 127, 255, 212, 255);
				}
				else if (t == EntityType::Point)
				{
					SDL_SetRenderDrawColor(_renderer, 0, 0, 50, 255);
				}
				else if (t == EntityType::Flag)
				{
					SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
				}
			}

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
						worldPoint = _systemManager.GetCamera().worldToScreen(worldPoint);
						points[i].x = worldPoint.x;
						points[i].y = worldPoint.y;
					}

					points[lenght].y = points[0].y;
					points[lenght].x = points[0].x;

					/*
					if (lenght > 2)
					{
						int width = points[1].x - points[0].x;
						int height = points[2].y - points[1].y;
						SDL_Rect rect = SDL_Rect{ points[0].x,points[0].y,width,height };
						SDL_RenderFillRect(_renderer, &rect);
					}*/
					
					SDL_RenderDrawLines(_renderer, points, lenght + 1);
					delete points;
				}
			}
		}
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}