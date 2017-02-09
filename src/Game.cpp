#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "Game.h"
#include "ConstHolder.h"

#include "LTimer.h"



Game::Game() 
	: _running(false)
	, _textureHolder(std::map<TextureID, SDL_Texture*>())
	, _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem(_renderer, &_cameraSystem.getCamera())
	, _physicsSystem()
	, _controlSystem()
	, _gravity(0.f, -9.8f)
	, _world(_gravity)
	, _contactListener(MyContactListener())
{
	_world.SetContactListener(&_contactListener);
	_world.SetAllowSleeping(false);
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

		Entity* player = new Entity(Entity::Type::Player);

		SpriteComponent* spriteComponent = new SpriteComponent(_textureHolder[TextureID::Player]);
		player->AddComponent(new BoundsComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
		player->AddComponent(spriteComponent);
		player->AddComponent(new PhysicsComponent(0.f, 0.f, 2.f, 2.f));
		_entities.push_back(player);
		_renderSystem.AddEntity(_entities.back());
		_cameraSystem.AddEntity(_entities.back());
		_physicsSystem.AddEntity(_entities.back());

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
	_textureHolder[TextureID::Player] = loadTexture("Media/Player/player.png");
	_textureHolder[TextureID::EntitySpriteSheet] = loadTexture("Media/Textures/EntitySprite.png");

	_levelLoader.LoadJson("Media/Json/Map.json",_entities,_renderSystem, _textureHolder);
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
	_world.Step(1 / (float)SCREEN_FPS, 8, 3);

	//save the curent time for next frame
	_lastTime = currentTime;
}

void Game::Render()
{
	SDL_RenderClear(_renderer);

	//test background in order to see the camera is following the player position

	//RENDER HERE
	_renderSystem.Process();

	//SDL_SetRenderDrawColor(_renderer, 0, 55, 55, 255);
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
		delete _entities[i];
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