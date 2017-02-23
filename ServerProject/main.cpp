#include "Net.h"
#include "Session.h"
#include <unordered_map>

using namespace Network;

typedef std::unordered_map<int, Session> SessionMap;


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

SessionListData CreateSessionListData(const SessionMap& sessions)
{
	SessionListData data;
	data.count = sessions.size();
	data.maxPlayers = MAX_PLAYERS;
	for (SessionMap::const_iterator it = sessions.begin(); it != sessions.end(); ++it)
	{
		data.sessionIDs.push_back(it->first);
		data.currentPlayers.push_back(it->second.GetPlayerCount());
	}

	return data;
}

void RemoveSpectator(std::vector<Spectator>& spectators, IPaddress srcAddr)
{
	//remove from spectator list
	spectators.erase(std::remove_if(spectators.begin(), spectators.end(),
		[srcAddr](const Spectator& s)
	{
		return (s.address.host == srcAddr.host && s.address.port == srcAddr.port);
	}
	), spectators.end());
}

int main(int argc, char** argv)
{

	SDLNet_Init();

	std::vector<Spectator> spectators;
	SessionMap sessions;

	Net net(5228);

	int playerCount = 0;
	int sessionCount = 0;
	while (true)
	{
		ReceivedData receiveData = net.Receive();

		if (receiveData.Empty() == false)
		{
			IPaddress srcAddr = receiveData.GetSrcAddress();
			switch (receiveData.GetType())
			{
			case MessageType::Connect:
			{
				ConnectData data = receiveData.GetData<ConnectData>();
				std::cout << "spectators " << spectators.size() << std::endl;
				if (exists(spectators, srcAddr) == false)//if the server isnt restarted and the same player trys to join again this isnt true
				{
					std::cout << "Player connected to server. ID: " << data.id << std::endl;
					if (data.id <= 0)
					{
						data.id = playerCount++;
						std::cout << "Spectator added. ID assigned: " << data.id << std::endl;
						Spectator s;
						s.playerID = data.id;
						s.address = srcAddr;
						spectators.push_back(s);

						net.Send(&data, srcAddr);
					}
				}
				std::cout << "Sending session list to player: " << data.id << std::endl;
				SessionListData sessionData = CreateSessionListData(sessions);
				net.Send(&sessionData, srcAddr);
				break;
			}
			case MessageType::JoinSession:
			{
				JoinSessionData data = receiveData.GetData<JoinSessionData>();
				if (data.sessionID >= 0) //session exists
				{
					if (sessions[data.sessionID].Joinable()) //has space to join
					{
						std::cout << "Player " << data.id << " joining session " << data.sessionID << std::endl;
						sessions[data.sessionID].AddPlayer(data.id, srcAddr);
					}
					else 
					{
						//client will ensure to check session list before trying to join a session in case it is full
						std::cout << "Can't join session as it is full..." << std::endl;
					}
				}
				else //need to make a new session
				{
					int sessionID = sessionCount++;
					std::cout << "Creating session " << sessionID << " with player " << data.id << " as host." << std::endl;
					//error check if player already in session.
					Session s;
					s.AddPlayer(data.id, srcAddr);
					sessions.insert(std::pair<int, Session>(sessionID, s));

					//now tell the player what session they have just joined and the player list?. player list is in different scene so that scene could send a seperate message
					data.sessionID = sessionID;
				}
				RemoveSpectator(spectators, srcAddr);

				net.Send(&data, srcAddr);
				break;
			}
			case MessageType::Disconnect:
				DisconnectData data = receiveData.GetData<DisconnectData>();
				std::cout << "Disconnecting player: " << data.id << " from session: " << data.sessionID << "which has " << sessions[data.sessionID].GetPlayerCount() << " players." << std::endl;
				if (data.id >= 0) //is a valid id
				{
					std::cout << "spectators " << spectators.size() << std::endl;
					if (data.sessionID >= 0) //is within a session
					{
						std::cout << "Removing a player from a session." << std::endl;
						if (sessions[data.sessionID].RemovePlayer(data.id))
						{
							//removed a host
							//need to let new host know
							int newHostId = sessions[data.sessionID].GetHostID();
							std::cout << "Removed " << data.id << " as host. New host is: " << newHostId << std::endl;
							if (newHostId >= 0)
							{
								SetHostData shdata;
								net.Send(&shdata, sessions[data.sessionID].GetPlayerIP(newHostId));
							}
						}
						if (sessions[data.sessionID].GetPlayerCount() == 0)
						{
							std::cout << "Session: " << data.sessionID << " has 0 players and will be removed." << std::endl;
							sessions.erase(sessions.find(data.sessionID));
						}
					}
					else //only a spectator
					{
						std::cout << "Removed spectator." << std::endl;
						RemoveSpectator(spectators, srcAddr);
					}
				}
				break;
			}
		}
	};

	SDLNet_Quit();
	return 0;
}