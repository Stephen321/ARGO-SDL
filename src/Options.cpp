#include "Options.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

Options::Options()
	: _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _functionMaster()
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

	Start();
	LoadContent();

	//Input
	BindInput();
}

int Options::Update()
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

void Options::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
	_renderSystem.Process();

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

}

void Options::CleanUp()
{

}

void Options::BindInput()
{
	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}