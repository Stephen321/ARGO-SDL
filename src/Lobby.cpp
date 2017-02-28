#include "Lobby.h"

#include <string>
#include <sstream>

Lobby::Lobby()
	: _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _functionMaster()
	, _uiSystem(0)
{
	_renderSystem.Initialize(_renderer, &_cameraSystem.getCamera());
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Lobby::~Lobby()
{
}

void Lobby::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;
	_running = true;

	_selectedItemIndex = 0;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// UI
	_uiSystem.Initialize(_renderer);

	Entity* ui = new Entity(EntityType::UI);
	_uiSystem.AddEntity(ui);

	// Scene
	//Start(); - activate when loaded
	LoadContent();

	//Input
	BindInput();

	Refresh();
}

int Lobby::Update()
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

void Lobby::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
	_renderSystem.Process();
	_uiSystem.Process();

	SDL_RenderPresent(_renderer);
}

bool Lobby::IsRunning()
{
	if (_swapScene != CurrentScene::LOBBY) { _swapScene = CurrentScene::LOBBY; }
	return _running;
}

void Lobby::Start()
{
	_swapScene = CurrentScene::LOBBY;
}

void Lobby::Stop()
{
	_running = false;
	CleanUp();
}

void Lobby::OnEvent(EventListener::Event evt)
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

void Lobby::LoadContent()
{
	_uiSystem.CreateDisplayText("Sessions", SCREEN_WIDTH / 2, 50);
	_uiSystem.CreateDisplayText("________", SCREEN_WIDTH / 2, 60);
}

void Lobby::CleanUp()
{

}

void Lobby::BindInput()
{
	Command* enterIn = new InputCommand([&]()
	{
		if (_selectedItemIndex == _uiSystem.GetInteractiveTextRectangle().size() - 1) { _running = false; }
		else { _swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1); }
	}, Type::Press);

	_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* pIn = new InputCommand([&]()
	{
		_uiSystem.DeleteText();
	}, Type::Press);

	_inputManager->AddKey(Event::p, pIn, this);

	Command* oIn = new InputCommand([&]()
	{
		Refresh();
	}, Type::Press);

	_inputManager->AddKey(Event::o, oIn, this);


	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void Lobby::MoveUp()
{
	if (_selectedItemIndex - 1 >= 0)
	{
		_selectedItemIndex--;
	}
	// Jump to bottom
	else
	{
		// _textRectangle.size() - 2 = 1 before icon
		_selectedItemIndex = _uiSystem.GetInteractiveTextRectangle().size() - 1;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}

void Lobby::MoveDown()
{
	if (_selectedItemIndex < _uiSystem.GetInteractiveTextRectangle().size() - 1)
	{
		_selectedItemIndex++;
	}
	// Jump to top
	else
	{
		_selectedItemIndex = 0;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}

int Lobby::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}

void Lobby::Refresh()
{
	_uiSystem.DeleteText();

	int amountOfLobbiesTest = 5;

	for (int i = 0; i < amountOfLobbiesTest; i++)
	{
		std::ostringstream oss;
		oss << "[" << i << "]" << " - " << i << "/" << 4;
		std::string var = oss.str();

		if (i == 0)
		{
			_uiSystem.CreateText(var, 50, 200);
		}

		else
		{
			_uiSystem.CreateText(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50);
		}
	}
}
