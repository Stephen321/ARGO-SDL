#include "About.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

About::About()
	: _uiSystem(0)
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

About::~About()
{
}

void About::Initialize(SDL_Renderer* renderer)
{
	Scene::Initialize(renderer);
	
	Start();

	//Input
	BindInput();
}

int About::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update

	// UPDATE HERE //
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();

	//save the curent time for next frame
	_lastTime = currentTime;

	return (int)_swapScene;
}

void About::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
	_uiSystem.Process();

	SDL_RenderPresent(_renderer);
}

bool About::IsRunning()
{
	if (_swapScene != CurrentScene::ABOUT) { _swapScene = CurrentScene::ABOUT; }
	return _running;
}

void About::Start()
{
	_running = true;
	_swapScene = CurrentScene::ABOUT;
}

void About::Stop()
{
	_running = false;
	CleanUp();
}

void About::OnEvent(Event evt, Type typ)
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

void About::BindInput()
{
	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void About::LoadContent()
{

}

void About::CleanUp()
{
	_inputManager->EmptyKeys();
	_uiSystem.DeleteDisplayText();
	_uiSystem.DeleteText();
	_uiSystem.DeleteEntites();
}