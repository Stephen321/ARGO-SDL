#include <iostream>
#include <thread>
#include <string>

#include "LTimer.h"
#include "ConstHolder.h"

#include "SceneManager.h"
#include "Game.h"

using namespace std;

const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

#undef main

int main(int argc, char** argv)
{
	srand(time(NULL));
	SceneManager* sceneManager = new SceneManager();

	//Adjust screen positions as needed
	DEBUG_MSG("Game Initialising");
	sceneManager->Initialize("Argo",100,100,1600,900, SDL_WINDOW_INPUT_FOCUS);

	DEBUG_MSG("Game Loop Starting......");
	LTimer capTimer;//to cap framerate

	while (sceneManager->IsRunning())
	{
		capTimer.start();

		sceneManager->Update();

		int frameTicks = capTimer.getTicks();//time since start of frame

		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	DEBUG_MSG("Calling Cleanup");
	game->CleanUp();

	//system("PAUSE");
	
	return 0;
}


