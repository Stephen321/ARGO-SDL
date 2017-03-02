#pragma once
#include <map>
#include <algorithm>
#include <SDL2/SDL_net.h>
#include <vector>

const int MAX_PLAYERS = 4;
const float TIME_OUT = 2000000000.f; //2 billion nanoseconds == 2 seconds

class Session {
public:
	Session();
	int GetPlayerCount() const;
	void AddPlayer(int playerID, IPaddress addr);
	bool RemovePlayer(int playerID);
	int GetHostID();
	IPaddress GetPlayerIP(int playerID);
	bool Joinable() const;
	std::vector<int> GetPlayerIDs() const;
	void Ready(int playerID);
	bool AllReady() const;
	std::vector<bool> GetReadied() const;
	int id;
private:
	struct Player
	{
		IPaddress address;
		bool ready;
	};
	std::map<int, Player> _players;
	bool _waiting;
	int _hostID;
};
