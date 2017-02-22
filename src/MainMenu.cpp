#include "MainMenu.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

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

void MainMenu::Initialize()
{
	_running = true;
	_swapScene = CurrentScene::MAIN_MENU;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	_uiSystem.Initialize(_renderer);

	// Text
	_selectedItemIndex = 0;

	_uiSystem.CreateText("Arcade Mode", 25, 450);
	//_uiSystem.CreateText("Multiplayer",	_textRectangle[0].x + _textRectangle[0].w / 2, _textRectangle[0].y + 100);
	//_uiSystem.CreateText("Options",		_textRectangle[1].x + _textRectangle[1].w / 2, _textRectangle[1].y + 100);
	//_uiSystem.CreateText("About",			_textRectangle[2].x + _textRectangle[2].w / 2, _textRectangle[2].y + 100);
	//_uiSystem.CreateText("Quit",			_textRectangle[3].x + _textRectangle[3].w / 2, _textRectangle[3].y + 100);

	//_uiSystem.CreateTextColoured(">", _textRectangle[0].x + _textRectangle[0].w + 50, _textRectangle[0].y, 255, 0, 0, 255);

	_uiSystem.UpdateText("Test", 0);

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

	_uiSystem.Process(dt);

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

	//// Text
	//for (int i = 0; i < _textTexture.size(); i++)
	//{
	//	SDL_RenderCopy(_renderer, _textTexture[i], NULL, &_textRectangle[i]);
	//}

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
	//Command* enterIn = new InputCommand([&]() 
	//{ 
	//	if (_selectedItemIndex + 1 == _textRectangle.size() - 1) { _running = false; }
	//	else { _swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1); }
	//}, Type::Press);

	//_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* sIn = new InputCommand([&]() { MainMenu::MoveDown(); }, Type::Press);
	_inputManager->AddKey(Event::s, sIn, this);

	Command* wIn = new InputCommand([&]() { MainMenu::MoveUp(); }, Type::Press);
	_inputManager->AddKey(Event::w, wIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void MainMenu::MoveUp()
{
	//if (_selectedItemIndex - 1 >= 0)
	//{
	//	_selectedItemIndex--;
	//}
	//// Jump to bottom
	//else
	//{
	//	// _textRectangle.size() - 2 = 1 before icon
	//	_selectedItemIndex = _textRectangle.size() - 2;
	//}
	//_textRectangle.back().x = _textRectangle[_selectedItemIndex].x + _textRectangle[_selectedItemIndex].w + 50;
	//_textRectangle.back().y = _textRectangle[_selectedItemIndex].y;
}

void MainMenu::MoveDown()
{
	//if (_selectedItemIndex + 1 < _textRectangle.size() - 1)
	//{
	//	_selectedItemIndex++;
	//}
	//// Jump to top
	//else
	//{
	//	_selectedItemIndex = 0;
	//}
	//_textRectangle.back().x = _textRectangle[_selectedItemIndex].x + _textRectangle[_selectedItemIndex].w + 50;
	//_textRectangle.back().y = _textRectangle[_selectedItemIndex].y;
}

int MainMenu::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}