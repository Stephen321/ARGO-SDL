#include "Session.h"

Session::Session(int maxPlayers)
	: MAX_PLAYERS(maxPlayers)
	, _playerCount(0)
	, _waiting(true)
{
}

int Session::GetPlayerCount() const
{
	return _playerCount;
}
