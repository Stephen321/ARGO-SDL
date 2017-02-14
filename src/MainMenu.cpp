#include "MainMenu.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

MainMenu::MainMenu()
	: _running(false)
	, _textureHolder(std::map<TextureID, SDL_Texture*>())
	, _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem(_renderer, &_cameraSystem.getCamera())
	, _controlSystem()
{

}


MainMenu::~MainMenu()
{
}

bool MainMenu::Initialize(SDL_Window* window, SDL_Renderer*	renderer, int width, int height)
{
	_running = SetupSDL(window, renderer);

	_cameraSystem.Init(width, height);
	if (_running)
	{//SETUP WHATEVER NEEDS TO BE 
		LoadContent();

		BindInput();

		TTF_Init();
		_fontSize = 32;
		_font = TTF_OpenFont("Media\\Fonts\\font.ttf", _fontSize);
		_surface = std::vector<SDL_Surface*>();
		_textTexture = std::vector<SDL_Texture*>();
		_textRectangle = std::vector<SDL_Rect>();
		_selectedItemIndex = 0;

		CreateText("Arcade Mode", 25, 450);
		CreateText("Multiplayer",	_textRectangle[0].x + _textRectangle[0].w / 2, _textRectangle[0].y + 100);
		CreateText("Options",		_textRectangle[1].x + _textRectangle[1].w / 2, _textRectangle[1].y + 100);
		CreateText("About",			_textRectangle[2].x + _textRectangle[2].w / 2, _textRectangle[2].y + 100);
		CreateText("Quit",			_textRectangle[3].x + _textRectangle[3].w / 2, _textRectangle[3].y + 100);

		CreateTextColoured(">", _textRectangle[0].x + _textRectangle[0].w + 50, _textRectangle[0].y, 255, 0, 0, 255);

		TTF_CloseFont(_font); // Free Font Memory

		_swapScene = CurrentScene::mainMenu;
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

	//test draw world bounds
	SDL_Rect r = { 0, 0, WORLD_WIDTH, WORLD_HEIGHT };
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(_renderer, &_cameraSystem.getCamera().worldToScreen(r));

	// Text
	for (int i = 0; i < _textTexture.size(); i++)
	{
		SDL_RenderCopy(_renderer, _textTexture[i], NULL, &_textRectangle[i]);
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

bool MainMenu::IsRunning()
{
	if (_swapScene != CurrentScene::mainMenu) { _swapScene = CurrentScene::mainMenu; }
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

void MainMenu::CreateText(string message, int x, int y)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ 255, 255, 255, 255 });
	_surface.push_back(surface);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, _surface.back());
	_textTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_textTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x; 
	textRectangle.y = y; 
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle.push_back(textRectangle);

	SDL_FreeSurface(_surface.back());
	SDL_RenderCopy(_renderer, _textTexture.back(), NULL, &_textRectangle.back());
}

void MainMenu::CreateTextColoured(string message, int x, int y, Uint8 r, Uint8 b, Uint8 g, Uint8 a)
{
	SDL_Surface* surface = TTF_RenderText_Blended(_font, message.c_str(), SDL_Color{ r, g, b, a });
	_surface.push_back(surface);

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, _surface.back());
	_textTexture.push_back(textTexture);

	int width, height;
	SDL_QueryTexture(_textTexture.back(), NULL, NULL, &width, &height);

	SDL_Rect textRectangle;
	textRectangle.x = x;
	textRectangle.y = y;
	textRectangle.w = width;
	textRectangle.h = height;
	_textRectangle.push_back(textRectangle);

	SDL_FreeSurface(_surface.back());
	SDL_RenderCopy(_renderer, _textTexture.back(), NULL, &_textRectangle.back());
}

void MainMenu::BindInput()
{
	Command* enterIn = new InputCommand([&]() { _swapScene = static_cast<CurrentScene>(1); }, Type::Press); //_selectedItemIndex + 1
	_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* sIn = new InputCommand([&]() { MainMenu::MoveDown(); }, Type::Press);
	_inputManager->AddKey(Event::s, sIn, this);

	Command* wIn = new InputCommand([&]() { MainMenu::MoveUp(); }, Type::Press);
	_inputManager->AddKey(Event::w, wIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void MainMenu::MoveUp()
{
	if (_selectedItemIndex - 1 >= 0)
	{
		_selectedItemIndex--;
		_textRectangle.back().x = _textRectangle[_selectedItemIndex].x + _textRectangle[_selectedItemIndex].w + 50;
		_textRectangle.back().y = _textRectangle[_selectedItemIndex].y;
	}
	// Jump to bottom
	else
	{
		_selectedItemIndex = _textRectangle.size() - 2;
		_textRectangle.back().x = _textRectangle[_textRectangle.size() - 2].x + _textRectangle[_textRectangle.size() - 2].w + 50;
		_textRectangle.back().y = _textRectangle[_textRectangle.size() - 2].y;
	}
}

void MainMenu::MoveDown()
{
	if (_selectedItemIndex + 1 < _textRectangle.size() - 1)
	{
		_selectedItemIndex++;
		_textRectangle.back().x = _textRectangle[_selectedItemIndex].x + _textRectangle[_selectedItemIndex].w + 50;
		_textRectangle.back().y = _textRectangle[_selectedItemIndex].y;
	}
	// Jump to top
	else
	{
		_selectedItemIndex = 0;
		_textRectangle.back().x = _textRectangle[_selectedItemIndex].x + _textRectangle[_selectedItemIndex].w + 50;
		_textRectangle.back().y = _textRectangle[_selectedItemIndex].y;
	}
}

int MainMenu::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}