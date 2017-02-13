#include "SDL.h"
#include "SDL_net.h"
#include <string>
#include <iostream>

#undef main

//int main(int argc, char** argv)
//{
//	SDL_Init(SDL_INIT_EVERYTHING);
//	SDLNet_Init();
//
//	IPaddress serverIp;
//	SDLNet_ResolveHost(&serverIp, "127.0.0.1", 4023);
//
//	UDPsocket socket = SDLNet_UDP_Open(4000);
//
//	UDPpacket* packet = SDLNet_AllocPacket(256);
//	packet->address.host = serverIp.host;
//	packet->address.port = serverIp.port;
//
//	std::string msg;
//
//
//	while (true)
//	{
//		std::cout << "Message to send: " << std::endl;
//		std::cin >> msg;
//		const char* data = msg.c_str();
//		packet->len = strlen(data) + 1;
//		memcpy(packet->data, data, packet->len);
//		if (!SDLNet_UDP_Send(socket, -1, packet))
//			std::cout << "error sending" << std::endl;
//	}
//
//	SDLNet_Quit();
//	SDL_Quit();
//	return 0;
//}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	
	UDPsocket socket = SDLNet_UDP_Open(4023);

	UDPpacket* packet = SDLNet_AllocPacket(256);

	std::string msg;

	int count = 0;
	while (true)
	{
		if (count > 100000)
		{
			std::cout << "waiting for message....." << std::endl;
			count = 0;
		}
		count++;


		if (SDLNet_UDP_Recv(socket, packet) > 0)
		{
			char* data = (char*)packet->data;
			std::cout << "mesaged received: " << data << std::endl;
		}
	}

	SDLNet_Quit();
	SDL_Quit();
	return 0;
}