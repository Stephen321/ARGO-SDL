//#include <SDL2/SDL.h>
//#include "Net.h"
//
//int main(int argc, char** argv)
//{
//	SDLNet_Init();
//
//	Net net(4000);
//
//	Net::ConnectData sendData;
//	sendData.s = "hello!";
//	sendData.ts = 100.f;
//	net.Send(&sendData, "192.168.15.38", 4023);
//
//	bool waiting = true;
//	while (waiting)
//	{
//		Net::ReceivedData receiveData = net.Receive();
//		if (receiveData.Empty() == false)
//		{
//			switch (receiveData.GetType())
//			{
//			case Net::MessageType::Connect:
//				Net::ConnectData* data = receiveData.GetData<Net::ConnectData>();
//				IPaddress srcAddress = data->srcAddress;
//				std::cout << "Received from: " << srcAddress.host << ":" << srcAddress.port << std::endl;
//				std::cout << data->s.c_str() << std::endl;
//				waiting = false;
//			}
//		}
//	};
//
//	system("PAUSE");
//	SDLNet_Quit();
//	return 0;
//}