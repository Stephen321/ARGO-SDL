#include <SDL2/SDL.h>
#include "Net.h"

#undef main
int main(int argc, char** argv)
{
	SDLNet_Init();

	Net net("127.0.0.1");

	Net::ConnectData data;
	data.s = "hello!";
	data.ts = 100.f;
	net.Send(&data);

	bool waiting = true;
	while (waiting)
	{
		Net::ReceivedData receiveData = net.Receive();
		if (receiveData.Empty() == false) 
		{ 
			switch (receiveData.GetType())
			{
			case Net::MessageType::Connect:
				Net::ConnectData* cd = receiveData.GetData<Net::ConnectData>();
				std::cout << cd->s.c_str() << std::endl;
				waiting = false;
			}
		}
	};

	system("PAUSE");
   	SDLNet_Quit();
	return 0;
}