#include "Options.h"

#include "ConstHolder.h"

#include <string>
#include <sstream>

Options::Options()
	: _uiSystem(0)
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Options::~Options()
{
}

void Options::Initialize(SDL_Renderer* renderer)
{
	Scene::Initialize(renderer);
	//UI
	_uiSystem.Initialize(_renderer);

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

void Options::OnEvent(Event evt, Type typ)
{
	if (_running)
	{

		switch (typ)
		{
		case Type::Press:
			switch (evt)
			{
			case Event::ESCAPE:
				_running = false;
				break;
			//case Event::w:
			//	_audioManager->PlayFX("Click");
			//	break;
			//case Event::s:
			//	_audioManager->PlayFX("Click");
			//	break;
			}
			break;

		default:
			break;
		}
	}
}

void Options::LoadContent()
{
	_uiSystem.CreateDisplayText("Options", SCREEN_WIDTH / 2, 50);
	_uiSystem.CreateDisplayText("_______", SCREEN_WIDTH / 2, 60);

	// Music Volume 0 -2
	_uiSystem.CreateDisplayText("Music Volume    =    ", SCREEN_WIDTH / 4, 200);
	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 200);
	_uiSystem.CreateTextAtCenter(GetMusicVolume(), SCREEN_WIDTH / 2, 200);
	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 200);

	// Hum 3 - 5
	_uiSystem.CreateDisplayText("Hum Volume    =    ", SCREEN_WIDTH / 4, 300);
	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 300);
	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 300);
	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 300);

	// Weapons 6 - 8
	_uiSystem.CreateDisplayText("Weapon Volume    =    ", SCREEN_WIDTH / 4, 400);
	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 400);
	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 400);
	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 400);

	// Checkpoint 9 - 11
	_uiSystem.CreateDisplayText("Checkpoint Volume    =    ", SCREEN_WIDTH / 4, 500);
	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 500);
	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 500);
	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 500);

	// Collision 12 - 14
	_uiSystem.CreateDisplayText("Collision Volume    =    ", SCREEN_WIDTH / 4, 600);
	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 600);
	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 600);
	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 600);

	// UI 15 - 17
	_uiSystem.CreateDisplayText("UI Volume    =    ", SCREEN_WIDTH / 4, 700);
	_uiSystem.CreateTextAtCenter("<", SCREEN_WIDTH / 2 - 100, 700);
	_uiSystem.CreateTextAtCenter(GetHumVolume(), SCREEN_WIDTH / 2, 700);
	_uiSystem.CreateTextAtCenter(">", SCREEN_WIDTH / 2 + 100, 700);
}

void Options::CleanUp()
{
	_inputManager->EmptyKeys();
	_uiSystem.DeleteDisplayText();
	_uiSystem.DeleteText();
	_uiSystem.DeleteEntites();
}

void Options::BindInput()
{
	Command* mlIn = new InputCommand([&]()
	{
		SDL_Point mousePos = _inputManager->GetMousePos();
		SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

		// Music
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[0]))) 
		{ 
			_audioManager->SetMusicVolume(false); 
			_uiSystem.UpdateTextAtCenter(GetMusicVolume(), 1);
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[2])))
		{ 
			_audioManager->SetMusicVolume(true);
			_uiSystem.UpdateTextAtCenter(GetMusicVolume(), 1);
		}

		// Hum
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[3])))
		{
			_audioManager->SetHumVolume(false);
			_uiSystem.UpdateTextAtCenter(GetHumVolume(), 4);
			_audioManager->PlayFX("Hum");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[5])))
		{
			_audioManager->SetHumVolume(true);
			_uiSystem.UpdateTextAtCenter(GetHumVolume(), 4);
			_audioManager->PlayFX("Hum");
		}

		// Weapons
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[6])))
		{
			_audioManager->SetWeaponVolume(false);
			_uiSystem.UpdateTextAtCenter(GetWeaponVolume(), 7);
			_audioManager->PlayFX("Weapon");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[8])))
		{
			_audioManager->SetWeaponVolume(true);
			_uiSystem.UpdateTextAtCenter(GetWeaponVolume(), 7);
			_audioManager->PlayFX("Weapon");
		}

		// Weapons
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[9])))
		{
			_audioManager->SetCheckpointVolume(false);
			_uiSystem.UpdateTextAtCenter(GetCheckpointVolume(), 10);
			_audioManager->PlayFX("Checkpoint");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[11])))
		{
			_audioManager->SetCheckpointVolume(true);
			_uiSystem.UpdateTextAtCenter(GetCheckpointVolume(), 10);
			_audioManager->PlayFX("Checkpoint");
		}

		// Collisions
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[12])))
		{
			_audioManager->SetCollisionVolume(false);
			_uiSystem.UpdateTextAtCenter(GetCollisionVolume(), 13);
			_audioManager->PlayFX("Collision");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[14])))
		{
			_audioManager->SetCollisionVolume(true);
			_uiSystem.UpdateTextAtCenter(GetCollisionVolume(), 13);
			_audioManager->PlayFX("Collision");
		}

		// UI
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[15])))
		{
			_audioManager->SetUIVolume(false);
			_uiSystem.UpdateTextAtCenter(GetUIVolume(), 16);
			_audioManager->PlayFX("Enter");
		}

		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[17])))
		{
			_audioManager->SetUIVolume(true);
			_uiSystem.UpdateTextAtCenter(GetUIVolume(), 16);
			_audioManager->PlayFX("Enter");
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
		_selectedItemIndex = _uiSystem.GetInteractiveTextRectangle().size() - 1;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}

void Options::MoveDown()
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

int Options::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}

std::string Options::GetMusicVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetMusicVolume();
	return oss.str();
}

std::string Options::GetHumVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetHumVolume();
	return oss.str();
}

std::string Options::GetWeaponVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetWeaponVolume();
	return oss.str();
}

std::string Options::GetCheckpointVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetCheckpointVolume();
	return oss.str();
}

std::string Options::GetCollisionVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetCollisionVolume();
	return oss.str();
}

std::string Options::GetUIVolume()
{
	std::ostringstream oss;
	oss << _audioManager->GetUIVolume();
	return oss.str();
}