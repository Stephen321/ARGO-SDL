#include "About.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

About::About()
	: _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _functionMaster()
{
	_renderSystem.Initialize(_renderer, &_cameraSystem.getCamera());
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

About::~About()
{
}

void About::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;
	_running = true;
	_swapScene = CurrentScene::ABOUT;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

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
	_renderSystem.Process();

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
	_swapScene = CurrentScene::LOBBY;
}

void About::Stop()
{
	_running = false;
	CleanUp();
}

void About::OnEvent(EventListener::Event evt)
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

}