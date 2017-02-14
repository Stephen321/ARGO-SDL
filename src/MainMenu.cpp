#include "MainMenu.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

MainMenu::MainMenu()
	: _running(false)
	, _textureHolder(std::map<TextureID, SDL_Texture*>())
	, _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _controlSystem()
{
	_renderSystem.Initialize(_renderer, &_cameraSystem.getCamera());
}


MainMenu::~MainMenu()
{
}

bool MainMenu::Initialize(SDL_Window* window, SDL_Renderer*	renderer, int width, int height)
{
	_running = SetupSDL(window, renderer);

	_cameraSystem.Initialize(width, height);
	if (_running)
	{//SETUP WHATEVER NEEDS TO BE 
		LoadContent();

		BindInput();

		SetupText(32, "Press Enter to go to Game", 100, 100);

		_swapScene = CurrentScene::MAIN_MENU;
	}

	return _running;
}

bool MainMenu::SetupSDL(SDL_Window*	window, SDL_Renderer* renderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		DEBUG_MSG("SDL Init success");
		_window = window;

		if (_window != 0)
		{
			DEBUG_MSG("Window creation success");
			_renderer = renderer;
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

void MainMenu::LoadContent()
{
	//_textureHolder[TextureID::TilemapSpriteSheet] = loadTexture("Media/Textures/BackgroundSprite.png");

	//_levelLoader.LoadJson("Media/Json/Map.json", &_entities, _entityFactory, _bodyFactory);
	//_levelLoader.LoadJson("Media/Json/Map2.json", _entities, _renderSystem, _textureHolder);
}

int MainMenu::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update
	
	//UPDATE HERE

	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();



	//save the curent time for next frame
	_lastTime = currentTime;

	return _swapScene;
}

void MainMenu::Render()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);

	//test background in order to see the camera is following the player position

	//RENDER HERE
	_renderSystem.Process();
	DebugText();

	//test draw world bounds
	SDL_Rect r = { 0, 0, WORLD_WIDTH, WORLD_HEIGHT };
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(_renderer, &_cameraSystem.getCamera().worldToScreen(r));

	SDL_RenderCopy(_renderer, _textTexture, NULL, &_textRectangle);

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

bool MainMenu::IsRunning()
{
	if (_swapScene != CurrentScene::MAIN_MENU) { _swapScene = CurrentScene::MAIN_MENU; }
	return _running;
}


void MainMenu::OnEvent(EventListener::Event evt)
{
	if (_running)
	{
		switch (evt)
		{
		case Event::ESCAPE:
			_running = false;
		}
	}
}

void MainMenu::SetupText(int fontSize, string message, int x, int y)
{
	TTF_Init();
	_font = TTF_OpenFont("Media\\Fonts\\font.ttf", fontSize);

	_surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });

	_textTexture = SDL_CreateTextureFromSurface(_renderer, _surface);
	int width, height;

	SDL_QueryTexture(_textTexture, NULL, NULL, &width, &height);

	_textRectangle.x = x; 
	_textRectangle.y = y; 
	_textRectangle.w = width;
	_textRectangle.h = height;

	SDL_FreeSurface(_surface);

	TTF_CloseFont(_font);

	SDL_RenderCopy(_renderer, _textTexture, NULL, &_textRectangle);
}

void MainMenu::DebugText()
{

}

void MainMenu::BindInput()
{
	Command* enterIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::GAME; }, Type::Press);
	_inputManager->AddKey(Event::RETURN, enterIn, this);
}