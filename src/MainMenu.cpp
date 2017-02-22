#include "MainMenu.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"

MainMenu::MainMenu()
	: _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _functionMaster()
	, _uiSystem(0)
{
	_renderSystem.Initialize(_renderer, &_cameraSystem.getCamera());
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}


MainMenu::~MainMenu()
{
}

void MainMenu::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;
	_running = true;
	_swapScene = CurrentScene::MAIN_MENU;

	_selectedItemIndex = 0;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
	_uiSystem.Initialize(_renderer);


	SDL_Rect rect = SDL_Rect();
	rect.x = 0;
	rect.y = 0;
	rect.w = 1;
	rect.h = 1;


	Entity* ui = new Entity(EntityType::UI);
	//ui->AddComponent(new TransformComponent(rect, 1, 1));
	//ui->AddComponent(new SpriteComponent(_textureHolder[TextureID::UI]));
	_uiSystem.AddEntity(ui);


	// Text
	_uiSystem.CreateText("Arcade Mode",	25, 450);
	_uiSystem.CreateText("Multiplayer",		_uiSystem._textRectangle[0].x + _uiSystem._textRectangle[0].w / 2, _uiSystem._textRectangle[0].y + 100);
	_uiSystem.CreateText("Options",			_uiSystem._textRectangle[1].x + _uiSystem._textRectangle[1].w / 2, _uiSystem._textRectangle[1].y + 100);
	_uiSystem.CreateText("About",			_uiSystem._textRectangle[2].x + _uiSystem._textRectangle[2].w / 2, _uiSystem._textRectangle[2].y + 100);
	_uiSystem.CreateText("Quit",			_uiSystem._textRectangle[3].x + _uiSystem._textRectangle[3].w / 2, _uiSystem._textRectangle[3].y + 100);

	_uiSystem.CreateTextColoured(">",		_uiSystem._textRectangle[0].x + _uiSystem._textRectangle[0].w + 50, _uiSystem._textRectangle[0].y, 255, 0, 0, 255);

	//TTF_CloseFont(_font); // Free Font Memory
	
	//Input
	BindInput();
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

	return (int)_swapScene;
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

	_uiSystem.Process(0);

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

bool MainMenu::IsRunning()
{
	if (_swapScene != CurrentScene::MAIN_MENU) { _swapScene = CurrentScene::MAIN_MENU; }
	return _running;
}

void MainMenu::Start()
{

}

void MainMenu::Stop()
{

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

void MainMenu::LoadContent()
{

}

void MainMenu::CleanUp()
{
	//DESTROY HERE

	//SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}



void MainMenu::BindInput()
{
	Command* enterIn = new InputCommand([&]() 
	{ 
		if (_selectedItemIndex + 1 == _uiSystem._textRectangle.size() - 1) { _running = false; }
		else { _swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1); }
	}, Type::Press);

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
	}
	// Jump to bottom
	else
	{
		// _textRectangle.size() - 2 = 1 before icon
		_selectedItemIndex = _uiSystem._textRectangle.size() - 2;
	}
	_uiSystem._textRectangle.back().x = _uiSystem._textRectangle[_selectedItemIndex].x + _uiSystem._textRectangle[_selectedItemIndex].w + 50;
	_uiSystem._textRectangle.back().y = _uiSystem._textRectangle[_selectedItemIndex].y;
}

void MainMenu::MoveDown()
{
	if (_selectedItemIndex + 1 < _uiSystem._textRectangle.size() - 1)
	{
		_selectedItemIndex++;
	}
	// Jump to top
	else
	{
		_selectedItemIndex = 0;
	}
	_uiSystem._textRectangle.back().x = _uiSystem._textRectangle[_selectedItemIndex].x + _uiSystem._textRectangle[_selectedItemIndex].w + 50;
	_uiSystem._textRectangle.back().y = _uiSystem._textRectangle[_selectedItemIndex].y;
}

int MainMenu::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}