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

void Options::Initialize()
{
	_running = true;
	_swapScene = CurrentScene::OPTIONS;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Text
	//TTF_Init();
	//_fontSize = 32;
	//_font = TTF_OpenFont("Media\\Fonts\\font.ttf", _fontSize);
	//_surface = std::vector<SDL_Surface*>();
	//_textTexture = std::vector<SDL_Texture*>();
	//_textRectangle = std::vector<SDL_Rect>();

	//CreateText("Options", 25, 450);

	//TTF_CloseFont(_font); // Free Font Memory

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

bool Options::IsRunning()
{
	if (_swapScene != CurrentScene::OPTIONS) { _swapScene = CurrentScene::OPTIONS; }
	return _running;
}

void Options::Start()
{

}

void Options::Stop()
{

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
	//DESTROY HERE

	//SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}



void Options::BindInput()
{
	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}