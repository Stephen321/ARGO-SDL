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
	Player p;
	p.address = addr;
	p.ready = false;
	_players.insert(std::pair<int, Player>(playerID, p));
	for (std::map<int, Player>::iterator it = _players.begin(); it != _players.end(); ++it)
	{
		it->second.ready = false; //unready everybody when someone joins
	}
}

bool Session::RemovePlayer(int playerID)
{
	bool removingHost = false;
	std::map<int, Player>::const_iterator idToErase = _players.find(playerID);
	if (playerID == GetHostID()) //host is being removed!
	{
		removingHost = true;
	}
	if (idToErase != _players.end())
	{
		_players.erase(idToErase); //also removes ready value 
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
	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		if (it->first == playerID)
		{
			return it->second.address;
		}
	}
	return IPaddress();
}

bool Session::Joinable() const
{
	return GetPlayerCount() < MAX_PLAYERS;
}

std::vector<int> Session::GetPlayerIDs() const
{
	std::vector<int> players;

	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		players.push_back(it->first);
	}

	return players;
}

void Session::Ready(int playerID)
{
	_players[playerID].ready = true;
}

bool Session::AllReady() const
{
	bool allready = true;
	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		if (!it->second.ready)
		{
			allready = false;
			break;
		}
	}
	return allready;
}

std::vector<bool> Session::GetReadied() const
{
	std::vector<bool> readiedUp;
	for (std::map<int, Player>::const_iterator it = _players.begin(); it != _players.end(); ++it)
	{
		readiedUp.push_back(it->second.ready);
	}
	return readiedUp;
}