#include <iostream>
#include <thread>
#include <string>

#include "LTimer.h"
#include "Game.h"
#include "ConstHolder.h"

using namespace std;

const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

#undef main

int main(int argc, char** argv)
{
	srand(time(NULL));
	Game* game = new Game();

	//Adjust screen positions as needed
	DEBUG_MSG("Game Initialising");
	game->Initialize("Argo",100,100,1600,900, SDL_WINDOW_INPUT_FOCUS);

	DEBUG_MSG("Game Loop Starting......");
	LTimer capTimer;//to cap framerate


	while (game->IsRunning())
	{
		capTimer.start();

		game->Update();
		game->Render();

		int frameTicks = capTimer.getTicks();//time since start of frame

		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
	while(game->IsRunning())
	{
		game->Update();
		game->Render();
	}

	DEBUG_MSG("Calling Cleanup");
	game->CleanUp();


	//system("PAUSE");
	
	return 0;
}


