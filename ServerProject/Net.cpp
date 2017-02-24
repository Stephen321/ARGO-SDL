#include "Net.h"
#include <sstream>

Network::Net::Net(int port, int packetSize)
{
	_socket = SDLNet_UDP_Open(port);
	if (!_socket)
	{
		std::cout << "Failed to open socket on port " << port << std::endl;
	}
	_packet = SDLNet_AllocPacket(packetSize);
}

void Network::Net::Send(MessageData * data, const char * destHost, int destPort)
{
	IPaddress destAddr;
	SDLNet_ResolveHost(&destAddr, destHost, destPort);
	Send(data, destAddr);
}

void Network::Net::Send(MessageData * data, IPaddress destAddr)
{
	if (!data)
	{
		std::cout << "Send tried to send nullptr" << std::endl;
		return;
	}

	MessageType type = data->GetType();
	_packet->len = 0;
	WriteInt((Uint8)type);

	//write id and session id which always exists
	WriteInt(data->id);
	WriteInt(data->sessionID);

	int v = 0;
	switch (type)
	{
	case MessageType::Connect:
	{
		ConnectData* cdata = (ConnectData*)data;
		break;
	}
	case MessageType::Disconnect:
	{
		DisconnectData* ddata = (DisconnectData*)data;
		break;
	}
	case MessageType::State:
	{
		StateData* sdata = (StateData*)data;
		WriteFloat(sdata->xPos);
		WriteFloat(sdata->yPos);
		WriteFloat(sdata->xVel);
		WriteFloat(sdata->yVel);
		break;
	}
	case MessageType::SessionList:
	{
		SessionListData* sldata = (SessionListData*)data;
		WriteInt(sldata->count);
		WriteInt(sldata->maxPlayers);
		for (int i = 0; i < sldata->count; i++)
		{
			WriteInt(sldata->sessionIDs[i]);
			WriteInt(sldata->currentPlayers[i]);
		}
		break;
	}
	case MessageType::JoinSession:
	{
		JoinSessionData* jsdata = (JoinSessionData*)data;
		break;
	}
	case MessageType::PlayerList:
	{
		PlayerListData* pldata = (PlayerListData*)data;
		WriteInt(pldata->count);
		for (int i = 0; i < pldata->count; i++)
		{
			WriteInt(pldata->players[i]);
		}
		break;
	}
	}

	_packet->address.host = destAddr.host;
	_packet->address.port = destAddr.port;
	std::cout << "Sending a " << GetTypeAsString(type).c_str() << " packet to: " << destAddr.host << ":" << destAddr.port << std::endl;
	//_packet->len--;
	if (SDLNet_UDP_Send(_socket, -1, _packet) == 0)
		std::cout << "Failed to send packet." << std::endl;
}


Network::ReceivedData Network::Net::Receive()
{
	ReceivedData receiveData;
	if (SDLNet_UDP_Recv(_socket, _packet) > 0)
	{
		receiveData.SetSrcAddress(_packet->address);
		int byteOffset = 0;
		MessageType type = (MessageType)ReadInt(byteOffset);
		int id = ReadInt(byteOffset);
		int sessionID = ReadInt(byteOffset);
		receiveData.SetIDs(id, sessionID);

		switch (type)
		{
		case MessageType::Connect:
		{
			ConnectData data;
			receiveData.SetData(data);
			break;
		}
		case MessageType::Disconnect:
		{
			DisconnectData data;
			receiveData.SetData(data);
			break;

		}
		case MessageType::State:
		{
			StateData data;
			data.xPos = ReadFloat(byteOffset);
			data.yPos = ReadFloat(byteOffset);
			data.xVel = ReadFloat(byteOffset);
			data.yVel = ReadFloat(byteOffset);
			receiveData.SetData(data);
			break;
		}
		case MessageType::SessionList:
		{
			SessionListData data;
			data.count = ReadInt(byteOffset);
			data.maxPlayers = ReadInt(byteOffset);
			for (int i = 0; i < data.count; i++)
			{ 
				data.sessionIDs.push_back(ReadInt(byteOffset));
				data.currentPlayers.push_back(ReadInt(byteOffset));
			}
			receiveData.SetData(data);
			break;
		}
		case MessageType::JoinSession:
		{
			JoinSessionData data;
			receiveData.SetData(data);
			break;
		}
		case MessageType::SetHost:
		{
			SetHostData data;
			receiveData.SetData(data);
			break;
		}
		case MessageType::PlayerList:
		{
			PlayerListData data;
			data.count = ReadInt(byteOffset);
			for (int i = 0; i < data.count; i++)
			{
				data.players.push_back(ReadInt(byteOffset));
			}
			receiveData.SetData(data);
			break;
		}
		}
	}
	return receiveData;
}

void Network::Net::WriteInt(int value) 
{
	_packet->data[_packet->len++] = (value);		
	_packet->data[_packet->len++] = (value >> 0x08);
	_packet->data[_packet->len++] = (value >> 0x10);
	_packet->data[_packet->len++] = (value >> 0x18);
}

int Network::Net::ReadInt(int & byteOffset) 
{
	int value = _packet->data[byteOffset++];
	value |= _packet->data[byteOffset++] << 0x08;
	value |= _packet->data[byteOffset++] << 0x10;
	value |= _packet->data[byteOffset++] << 0x18;
	return value;
}

void Network::Net::WriteFloat(float valueF)
{
	//endianess issues? IEEE 754?
	Uint8* value = reinterpret_cast<Uint8*>(&valueF);
	for (int i = 0; i < sizeof(float); i++)
	{
		_packet->data[_packet->len++] = value[i];
	}
}

float Network::Net::ReadFloat(int & byteOffset)
{
	float value;
	memcpy(&value, _packet->data + byteOffset, sizeof(float));
	byteOffset += sizeof(float);
	return value;
}

void Network::Net::WriteString(std::string & s)
{
	int length = s.length() + 1;
	WriteInt(length); //this adds 4 more bytes? instead, try reading until nul is reached but depends on encoding
	memcpy(_packet->data + _packet->len, s.c_str(), length);
	_packet->len += length;
}

std::string Network::Net::ReadString(int & byteOffset)
{ 
	int length = ReadInt(byteOffset);
	std::stringstream ss;
	for (int i = 0; i < length; i++)
	{
		ss << _packet->data[byteOffset++];
	}
	return ss.str();
}

std::string Network::Net::GetTypeAsString(MessageType type)
{
	std::string s;

	switch (type)
	{
	case MessageType::Connect:
		s = "Connect";
		break;
	case MessageType::Disconnect:
		s = "Disconnect";
		break;
	case MessageType::SessionList:
		s = "SessionList";
		break;
	case MessageType::State:
		s = "State";
		break;
	case MessageType::JoinSession:
		s = "JoinSession";
		break;
	case MessageType::SetHost:
		s = "SetHost";
		break;
	}
	return s;
}

