#include "NetworkHandler.h"

void NetworkHandler::Disconnect()
{
	DisconnectData disconnect;
	disconnect.id = _playerID;
	disconnect.sessionID = _sessionID;
	Send(&disconnect);
	std::cout << "Disconnecting " << _playerID << std::endl;
}

NetworkHandler::NetworkHandler()
	: _net(PORT)
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
	_net.Send(data, _serverIP);
}

Network::ReceivedData NetworkHandler::Receive()
{
	return _net.Receive();
}
