#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string>
#include <iostream>
#include "Net.h"

#undef main
int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	Net net(256, "127.0.0.1");

	std::string msg;

	std::cout << "Message to send: " << std::endl;
	//std::cin >> msg;

	net.Send(Net::MessageType::Connect);

	//bool checkForMessage = true;
	//while (checkForMessage)
	//{
		//if (net.Receive())
		//{
		//	checkForMessage = false;
		//}
	//} 

   	SDLNet_Quit();
	SDL_Quit();
	system("PAUSE");
	return 0;
}


//#include <SDL2/SDL.h>
//#include <SDL2/SDL_net.h>
//#include <string>
//#include <iostream>
//
//int main(int argc, char** argv)
//{
//	SDL_Init(0);
//	SDLNet_Init();
//
//	UDPsocket socket = SDLNet_UDP_Open(4023);
//
//	UDPpacket* packet = SDLNet_AllocPacket(256);
//
//	std::string msg;
//
//	while (true)
//	{
//		if (SDLNet_UDP_Recv(socket, packet) > 0)
//		{
//			char* data = (char*)packet->data;
//			std::cout << "message received: " << data << " Port: " << packet->address.port << std::endl;
//			if (!SDLNet_UDP_Send(socket, -1, packet))
//				return -1;
//		}
//	}
//
//	SDLNet_Quit();
//	SDL_Quit();
//	return 0;
//}
