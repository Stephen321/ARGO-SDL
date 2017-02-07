#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "Game.h"

#include "LTimer.h"

#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"

#include "Factory.h"
using namespace rapidjson;

Game::Game() 
	: _running(false)
	, _textureHolder(std::map<TextureID, SDL_Texture*>())
{
}

Game::~Game()
{
}


bool Game::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	_running = SetupSDL(title, xpos, ypos, width, height, flags);
	
	if (_running)
	{//SETUP WHATEVER NEEDS TO BE SETUP
		LoadContent();


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
	Factory factory = Factory();
	char* filename = "Media/Json/Map.json";
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

	const Value& layerArray = doc["layers"];
	const Value& layer = layerArray[0u];
	const Value& dataArray = layer["data"];

	int layerWidth = layer["width"].GetInt();
	int layerHeight = layer["height"].GetInt();

	const Value& tilesetsArray = doc["tilesets"];
	const Value& tilesets = tilesetsArray[0u];
	const char* textureLoc = tilesets["image"].GetString();
	_textureHolder[TextureID::Wall] = IMG_LoadTexture(_renderer, textureLoc);
	if (_textureHolder[TextureID::Wall] == NULL)
	{
		std::cout << "ERROR LOADING : " << textureLoc << std::endl;
		system("PAUSE");
		exit(0);
	}
	int tileWidth = doc["tilewidth"].GetInt();
	int tileHeight = doc["tileheight"].GetInt();

	for (int y = 0; y < layerHeight; y++)
	{
		for (int x = 0; x < layerWidth; x++)
		{
			_renderSystem.AddEntity(factory.CreateWall(_textureHolder[TextureID::Wall],x * tileWidth,y * tileHeight,tileWidth,tileHeight));
		}
	}


	//_textureHolder[Textures::ID::Player] = IMG_LoadTexture(_renderer, "Media/Textures/Player.png");
}

void Game::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float deltaTime = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update



	//UPDATE HERE


	//save the curent time for next frame
	_lastTime = currentTime;
}

void Game::Render()
{
	SDL_RenderClear(_renderer);


	//RENDER HERE



	SDL_SetRenderDrawColor(_renderer, 55, 55, 55, 255);
	SDL_RenderPresent(_renderer);
}

void Game::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				_running = false;
				break;


			default:
				break;
			}
			break;
		}
	}
}

bool Game::IsRunning()
{
	return _running;
}


void Game::CleanUp()
{
	DEBUG_MSG("Cleaning Up");

	//DESTROY HERE


	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}
