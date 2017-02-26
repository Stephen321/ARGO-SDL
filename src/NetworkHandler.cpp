#include "NetworkHandler.h"

void NetworkHandler::Disconnect()
{
	if (_playerID < 0)
		return;
	DisconnectData disconnect;
	disconnect.id = _playerID;
	disconnect.sessionID = _sessionID;
	Send(&disconnect);
	std::cout << "Disconnecting " << _playerID << std::endl;
	_playerID = 0;
	_sessionID = -1;
}

NetworkHandler::NetworkHandler()
	: _net(6000)//PORT)
	, _playerID(-1)
	, _sessionID(-1)
{
	SDLNet_ResolveHost(&_serverIP, "localhost", 5228);
}

NetworkHandler::~NetworkHandler()
{
	if (GetConnected())
		Disconnect();
}

void NetworkHandler::Send(MessageData * data, const char * destHost, int destPort)
{
	_net.Send(data, destHost, destPort);
}

void NetworkHandler::Send(MessageData * data)
{
	data->id = _playerID;
	data->sessionID = _sessionID;
	_net.Send(data, _serverIP);
}

Network::ReceivedData NetworkHandler::Receive()
{
	return _net.Receive();
}
