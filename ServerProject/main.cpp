#include "Net.h"
#include <unordered_map>
#include <time.h>

using namespace Network;

typedef std::unordered_map<int, IPaddress>::const_iterator ConnectionIterator;
bool exists(const std::unordered_map<int, IPaddress>& connections, const IPaddress& ip)
{
	for (ConnectionIterator it = connections.begin(); it != connections.end(); ++it)
	{
		if (it->second.host == ip.host &&
			it->second.port == ip.port)
			return true;
	}
	return false;
}

bool exists(const std::unordered_map<int, IPaddress>& connections, int id)
{
	if (connections.find(id) != connections.end())
	{
		return true;
	}
	return false;
}

int main(int argc, char** argv)
{
	SDLNet_Init();

	std::unordered_map<int, IPaddress> connections;
	const int MaxPlayers = 8;

	Net net(5228);

	//thread pool for messages
	bool listening = true;
	while (listening)
	{
		ReceivedData receiveData = net.Receive();

		if (receiveData.Empty() == false)
		{
			switch (receiveData.GetType())
			{
			case MessageType::Connect:
			{
				IPaddress srcAddr = receiveData.GetData<ConnectData>()->srcAddress;
				if (exists(connections, srcAddr) == false && connections.size() < MaxPlayers)
				{
					int id = connections.size();
					std::cout << "Player " << id << " connected. (" << (id + 1) << "/" << MaxPlayers << "players)" << std::endl;
					connections.insert(std::pair<int, IPaddress>(id, srcAddr));
					ConnectData data;
					data.id = id;
					std::cout << "ID to send is " << data.id << std::endl;
					net.Send(&data, srcAddr);
				}
				break;
			}
			case MessageType::State:
			{
				StateData* data = receiveData.GetData<StateData>();
				if (exists(connections, data->id))
				{
					std::cout << "Player " << data->id << " state changed." << std::endl;
					for (ConnectionIterator it = connections.begin(); it != connections.end(); ++it)
					{
						if (it->first != data->id)
						{
							std::cout << "Sending state to " << it->first << "." << std::endl;
							net.Send(data, it->second);
						}
					}
				}
				break;
			}
			}
		}
	};

	system("PAUSE");
	SDLNet_Quit();
	return 0;
}