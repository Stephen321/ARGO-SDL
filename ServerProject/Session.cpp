#include "Session.h"

Session::Session()
	: _waiting(true)
{
}

int Session::GetPlayerCount() const
{
	return _players.size();
}

void Session::AddPlayer(int playerID, IPaddress addr)
{
	_players.insert(std::pair<int, IPaddress>(playerID, addr));
}

bool Session::RemovePlayer(int playerID)
{
	bool removingHost = false;
	std::map<int, IPaddress>::const_iterator idToErase = _players.find(playerID);
	if (playerID == GetHostID()) //host is being removed!
	{
		removingHost = true;
	}
	if (idToErase != _players.end())
	{
		_players.erase(idToErase);
	}
	return removingHost;
}

int Session::GetHostID()
{
	if (_players.empty())
	{
		return -1;
	}
	return _players.begin()->first;
}

IPaddress Session::GetPlayerIP(int playerID)
{
	for (std::map<int, IPaddress>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		if (it->first == playerID)
		{
			return it->second;
		}
	}
	return IPaddress();
}
