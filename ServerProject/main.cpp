#include "Net.h"
#include <unordered_map>
#include <time.h>
#include "Session.h"

using namespace Network;

typedef std::vector<int, IPaddress>::const_iterator SpectatorIterator;
bool exists(const std::vector<int, IPaddress>& spectators, const IPaddress& ip)
{
	for (SpectatorIterator it = spectators.begin(); it != spectators.end(); ++it)
	{
		if (it->second.host == ip.host &&
			it->second.port == ip.port)
			return true;
	}
	return false;
}

bool exists(const std::vector<int, IPaddress>& spectators, int id)
{
	for (SpectatorIterator it = spectators.begin(); it != spectators.end(); ++it)
	{
		if (it->first == id)
			return true;
	}
	return false;
}

int main(int argc, char** argv)
{
	typedef std::unordered_map<int, Session>::const_iterator SessionIterator;

	SDLNet_Init();

	const int MAX_PLAYERS = 4;
	std::vector<int, IPaddress> spectators;
	std::unordered_map<int, Session> sessions;

	Net net(5228);

	int playerCount = 0;
	while (true)
	{
		ReceivedData receiveData = net.Receive();

		if (receiveData.Empty() == false)
		{
			switch (receiveData.GetType())
			{
			case MessageType::Connect:
			{
				ConnectData* data = receiveData.GetData<ConnectData>();
				IPaddress srcAddr = data->srcAddress;

				std::cout << "Player connected to server. ID: " << data->id << std::endl;
				if (exists(spectators, srcAddr) == false)
				{
					if (data->id < 0)
					{
						data->id = playerCount++;
						std::cout << "Spectator added. ID assigned: " << data->id << std::endl;
					}
					spectators.push_back(std::pair<int, IPaddress>(data->id, srcAddr));
					SessionListData sessionData;
					sessionData.count = sessions.size();
					sessionData.maxPlayers = MAX_PLAYERS;
					for (SessionIterator it = sessions.begin(); it != sessions.end(); ++it)
					{
						sessionData.sessionIDs.push_back(it->first);
						sessionData.currentPlayers.push_back(it->second.GetPlayerCount());
					}
					net.Send(&sessionData, srcAddr);
				}
				break;
			}
			case MessageType::State:
			{
			}
			}
		}
	};

	system("PAUSE");
	SDLNet_Quit();
	return 0;
}