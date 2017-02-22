#pragma once
#include <unordered_map>

class Session {
public:
	Session(int maxPlayers);
	int GetPlayerCount() const;
private:
	const int MAX_PLAYERS;
	int _playerCount;
	bool _waiting;
};
