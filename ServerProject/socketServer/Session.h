#pragma once
#include <map>
#include <algorithm>
#include <SDL2/SDL_net.h>
#include <vector>

const int MAX_PLAYERS = 4;

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
private:
	std::map<int, IPaddress> _players;
	bool _waiting;
};
