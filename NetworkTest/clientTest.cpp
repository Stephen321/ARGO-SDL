#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string>
#include <iostream>

#undef main
int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	IPaddress serverIp;
	SDLNet_ResolveHost(&serverIp, "ec2-35-165-81-75.us-west-2.compute.amazonaws.com", 4023);

	UDPsocket socket = SDLNet_UDP_Open(4023);

	UDPpacket* packet = SDLNet_AllocPacket(256);
	packet->address.host = serverIp.host;
	packet->address.port = serverIp.port;

	std::string msg;


	std::cout << "Message to send: " << std::endl;
	std::cin >> msg;
	const char* data = msg.c_str();
	packet->len = strlen(data) + 1;
	memcpy(packet->data, data, packet->len);
	if (!SDLNet_UDP_Send(socket, -1, packet))
		std::cout << "error sending" << std::endl;


	bool checkForMessage = true;
	while (checkForMessage)
	{
		if (SDLNet_UDP_Recv(socket, packet) > 0)
		{
			checkForMessage = false;
			char* data = (char*)packet->data;
			std::cout << "mesaged received from server: " << data << std::endl;
		}
	}

	SDLNet_Quit();
	SDL_Quit();
	system("PAUSE");
	return 0;
}