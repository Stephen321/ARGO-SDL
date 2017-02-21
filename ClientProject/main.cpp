#include "Net.h"

using namespace Network;

int main(int argc, char** argv)
{
	SDLNet_Init();


	Net net(6000);

	system("PAUSE");
	SDLNet_Quit();
	return 0;
} 

