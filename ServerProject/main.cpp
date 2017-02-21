#include "Net.h"
#include "Session.h"

using namespace Network;

struct Spectator {
	int playerID;
	IPaddress address;
};

bool exists(const std::vector<Spectator>& spectators, const IPaddress& ip)
{
	for (std::vector<Spectator>::const_iterator it = spectators.begin(); it != spectators.end(); ++it)
	{
		if (it->address.host == ip.host &&
			it->address.port == ip.port)
			return true;
	}
	return false;
}

bool exists(const std::vector<Spectator>& spectators, int id)
{
	for (std::vector<Spectator>::const_iterator it = spectators.begin(); it != spectators.end(); ++it)
	{
		if (it->playerID == id)
			return true;
	}
	return false;
}

int main(int argc, char** argv)
{
	typedef std::unordered_map<int, Session> SessionMap;

	SDLNet_Init();

	const int MAX_PLAYERS = 4;
	std::vector<Spectator> spectators;
	SessionMap sessions;

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
					Spectator s;
					s.playerID = data->id;
					s.address = srcAddr;
					spectators.push_back(s);

					SessionListData sessionData;
					sessionData.count = sessions.size();
					sessionData.maxPlayers = MAX_PLAYERS;
					for (SessionMap::iterator it = sessions.begin(); it != sessions.end(); ++it)
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

	SDLNet_Quit();
	return 0;
}