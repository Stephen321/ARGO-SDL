#pragma once

class Net
{
public:
	enum class MessageType : Uint8 {
		Connect,
		Disconnect,
		State
	};

	struct MessageData {
		float ts;
		MessageType type;
		int length;
	};

	struct StateData : MessageData {
		StateData() 
		{
			type = MessageType::State;
		}
		float xPos;
		float yPos;
		float xVel;
		float yVel;
	};

	Net(int packetSize, const std::string& serverHost, int serverPort = 4023, int clientPort = 4000) 
	{
		SDLNet_ResolveHost(&_serverIP, serverHost.c_str(), serverPort);
		SetupPacket(packetSize);
		_socket = SDLNet_UDP_Open(clientPort);
	}

	//template<typename T>
	void Send(MessageType type, void* data = nullptr)
	{//set _packet.data + len
		_packet->len = 0;

		//set type into first byte as int
		WriteInt((Uint8)type, _packet->len);
		if (data != nullptr)
		{
			//add in data
		}
		if (SDLNet_UDP_Send(_socket, -1, _packet) == 0)
			std::cout << "Failed to send packet." << std::endl;
	}
	
	bool Receive()
	{
		if (SDLNet_UDP_Recv(_socket, _packet) > 0)
		{
			//pull out type
			//reconstruct struct

			char* data = (char*)_packet->data;
			std::cout << "mesaged received from server: " << data << std::endl;
			return true;
		}
		return false;
	}

private:
	void SetupPacket(int packetSize)
	{
		_packet = SDLNet_AllocPacket(packetSize);
		SetPacketAddress();
	}

	void SetPacketAddress()
	{
		_packet->address.host = _serverIP.host;
		_packet->address.port = _serverIP.port;
	}

	void WriteInt(int value, int byteOffset) //write to only 1 byte?
	{
		value = 2155905152;
		Uint8 t1 = (value);
		Uint8 t2 = (value >> 0x08);
		Uint8 t3 = (value >> 0x10);
		Uint8 t4 = (value >> 0x18);
		_packet->data[byteOffset] = value;
		_packet->data[byteOffset + 1] = value;
		_packet->data[byteOffset + 2] = value;
		_packet->data[byteOffset + 3] = value;
	}

	IPaddress _serverIP;
	UDPpacket* _packet;
	UDPsocket _socket;
};