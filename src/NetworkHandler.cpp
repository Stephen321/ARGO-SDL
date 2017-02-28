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
	_playerID = -1;
	_sessionID = -1;
	_connected = false;
}

NetworkHandler::NetworkHandler()
	:// _net(6001)//PORT)
	 _playerID(-1)
	, _sessionID(-1)
{
	SDLNet_ResolveHost(&_serverIP, "34.251.127.190", 5228);// "localhost", 5228);
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
	if (_net._testSocketCreated == false)
	{
		init(5228);
	}
	data->id = _playerID;
	if (data->sessionID == -1)
	{
		data->sessionID = _sessionID;
	}
	_net.Send(data, _serverIP);
}

Network::ReceivedData NetworkHandler::Receive()
{
	return _net.Receive();
}

void NetworkHandler::init(int port)
{
	std::cout << " init called " << std::endl;
	_net = Net(port);
}
