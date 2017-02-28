#include "Options.h"

#include <string>
#include <sstream>

Options::Options()
	: _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _functionMaster()
	, _uiSystem(0)
{
	_renderSystem.Initialize(_renderer, &_cameraSystem.getCamera());
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Options::~Options()
{
}

void Options::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//UI
	_uiSystem.Initialize(_renderer);

	Entity* ui = new Entity(EntityType::UI);
	_uiSystem.AddEntity(ui);

	// Scene
	Start();
	LoadContent();

	//Input
	BindInput();
}

int Options::Update()
{
	unsigned int currentTime = LTimer::gameTime();	//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update

	// UPDATE HERE	
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();

	//save the curent time for next frame
	_lastTime = currentTime;

	return (int)_swapScene;
}

void Options::Render()
{
	SDL_RenderClear(_renderer);

	// RENDER HERE
	_renderSystem.Process();
	_uiSystem.Process();

	SDL_RenderPresent(_renderer);
}

bool Options::IsRunning()
{
	if (_swapScene != CurrentScene::OPTIONS) { _swapScene = CurrentScene::OPTIONS; }
	return _running;
}

void Options::Start()
{
	_running = true;
	_swapScene = CurrentScene::OPTIONS;
}

void Options::Stop()
{
	_running = false;
	CleanUp();
}

void Options::OnEvent(EventListener::Event evt)
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

void Options::LoadContent()
{
	_uiSystem.CreateDisplayText("Options", SCREEN_WIDTH / 2, 50);
	_uiSystem.CreateDisplayText("_______", SCREEN_WIDTH / 2, 60);

	// Music Volume
	_uiSystem.CreateDisplayText("Music Volume    =    ", SCREEN_WIDTH / 4, 200);

	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 200);

	_uiSystem.CreateTextAtCenter(GetMusicVolume(), SCREEN_WIDTH / 2, 200);

	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 200);

	// Hum
	_uiSystem.CreateDisplayText("Hum Volume    =    ", SCREEN_WIDTH / 4, 300);

	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 300);

	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 300);

	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 300);

	// Fire
	_uiSystem.CreateDisplayText("Weapon Volume    =    ", SCREEN_WIDTH / 4, 400);

	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 400);

	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 400);

	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 400);

	// Collision
	_uiSystem.CreateDisplayText("Collision Volume    =    ", SCREEN_WIDTH / 4, 500);

	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 500);

	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 500);

	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 500);

	// Checkpoint
	_uiSystem.CreateDisplayText("Checkpoint Volume    =    ", SCREEN_WIDTH / 4, 600);

	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 600);

	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 600);

	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 600);
}

void Options::CleanUp()
{

}

void Options::BindInput()
{
	Command* mlIn = new InputCommand([&]()
	{
		SDL_Point mousePos = _inputManager->GetMousePos();
		SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem._interactiveTextRectangle[0]))) 
		{ 
			_audioManager->SetMusicVolume(false); 
			_uiSystem.UpdateTextAtCenter(GetMusicVolume(), 1);
			_audioManager->PlayFX("Hum");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem._interactiveTextRectangle[2]))) 
		{ 
			_audioManager->SetMusicVolume(true);
			_uiSystem.UpdateTextAtCenter(GetMusicVolume(), 1);
			_audioManager->PlayFX("Hum");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem._interactiveTextRectangle[3])))
		{
			_audioManager->SetHumVolume(false);
			_uiSystem.UpdateTextAtCenter(GetHumVolume(), 4);
			_audioManager->PlayFX("Hum");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem._interactiveTextRectangle[5])))
		{
			_audioManager->SetHumVolume(true);
			_uiSystem.UpdateTextAtCenter(GetHumVolume(), 4);
			_audioManager->PlayFX("Hum");
		}

	}, Type::Press);
	_inputManager->AddKey(Event::MOUSE_LEFT, mlIn, this);


	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void Options::MoveUp()
{
	if (_selectedItemIndex - 1 >= 0)
	{
		_selectedItemIndex--;
	}

	// Jump to bottom
	else
	{
		// _textRectangle.size() - 2 = 1 before icon
		_selectedItemIndex = _uiSystem._interactiveTextRectangle.size() - 1;
	}
	_uiSystem._displayTextRectangle.back().x = _uiSystem._interactiveTextRectangle[_selectedItemIndex].x + _uiSystem._interactiveTextRectangle[_selectedItemIndex].w + 50;
	_uiSystem._displayTextRectangle.back().y = _uiSystem._interactiveTextRectangle[_selectedItemIndex].y;
}

void Options::MoveDown()
{
	if (_selectedItemIndex < _uiSystem._interactiveTextRectangle.size() - 1)
	{
		_selectedItemIndex++;
	}

	// Jump to top
	else
	{
		_selectedItemIndex = 0;
	}
	_uiSystem._displayTextRectangle.back().x = _uiSystem._interactiveTextRectangle[_selectedItemIndex].x + _uiSystem._interactiveTextRectangle[_selectedItemIndex].w + 50;
	_uiSystem._displayTextRectangle.back().y = _uiSystem._interactiveTextRectangle[_selectedItemIndex].y;
}

int Options::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}

std::string Options::GetHumVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetHumVolume();
	return oss.str();
}

std::string Options::GetMusicVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetMusicVolume();
	return oss.str();
}
