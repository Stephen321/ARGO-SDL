#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL_net.h>

namespace Network
{
	enum class MessageType : Uint8 {
		Connect,
		Disconnect,
		State,
		SessionList
	};

	struct MessageData {
		float ts;
		int length;
		IPaddress srcAddress;
		MessageType GetType() const {
			return type;
		}
	protected:
		MessageType type;
	};

	struct StateData : MessageData {
		StateData() { type = MessageType::State; }
		int id;
		float xPos;
		float yPos;
		float xVel;
		float yVel;
	};
	struct ConnectData : MessageData {
		ConnectData() : id(-1), level(0) { type = MessageType::Connect; }
		int id;
		int level;
	};

	struct DisconnectData : MessageData {
		DisconnectData() { type = MessageType::Disconnect; }
	};

	struct SessionListData : MessageData {
		SessionListData() { type = MessageType::SessionList; }
		int count;
		int maxPlayers;
		std::vector<int> sessionIDs;
		std::vector<int> currentPlayers;
	};

	class ReceivedData {
	public:
		ReceivedData()
			: _data(nullptr)
		{
		}
		ReceivedData(const ReceivedData& rhs)
		{
			if (!rhs._data)
			{
				return;
			}
			switch (rhs._data->GetType())
			{ //template specialization to use enum type in order to determine Type
			case MessageType::Connect:
			{
				_data = new ConnectData(*rhs.GetData<ConnectData>());
				break;
			}
			case MessageType::Disconnect:
			{
				_data = new DisconnectData(*rhs.GetData<DisconnectData>());
				break;
			}
			case MessageType::State:
			{
				_data = new StateData(*rhs.GetData<StateData>());
				break;
			}
			}
		}

		template <typename T>
		void SetData(T data)
		{
			_data = new T(data);
		}

		~ReceivedData()
		{
			if (_data != nullptr)
			{
				delete _data;
				_data = nullptr;
			}
		}

		bool Empty() const
		{
			return _data == nullptr;
		}

		MessageType GetType() const
		{
			return _data->GetType();
		}

		template <typename T>
		T* GetData() const
		{
			return static_cast<T*>(_data);
		}

	private:
		MessageData* _data;
	};

	class Net
	{
	public:
		Net(int port, int packetSize = 256);

		void Send(MessageData* data, const char * destHost, int destPort)
		{
			IPaddress destAddr;
			SDLNet_ResolveHost(&destAddr, destHost, destPort);
			Send(data, destAddr);
		}

		void Send(MessageData* data, IPaddress destAddr)
		{
			if (!data)
			{
				std::cout << "Send tried to send nullptr" << std::endl;
				return;
			}

			MessageType type = data->GetType();
			_packet->len = 0;
			WriteInt((Uint8)type);

			int v = 0;
			switch (type)
			{
			case MessageType::Connect:
			{
				ConnectData* cdata = (ConnectData*)data;
				WriteInt(cdata->id);
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
				WriteInt(sdata->id);
				WriteFloat(sdata->xPos);
				WriteFloat(sdata->yPos);
				WriteFloat(sdata->xVel);
				WriteFloat(sdata->yVel);
				break;
			}
			}

			_packet->address.host = destAddr.host;
			_packet->address.port = destAddr.port;
			std::cout << "Sending to: " << destAddr.host << ":" << destAddr.port << std::endl;
			//_packet->len--;
			if (SDLNet_UDP_Send(_socket, -1, _packet) == 0)
				std::cout << "Failed to send packet." << std::endl;
		}

		ReceivedData Receive()
		{
			ReceivedData receiveData;
			if (SDLNet_UDP_Recv(_socket, _packet) > 0)
			{
				int byteOffset = 0;
				MessageType type = (MessageType)ReadInt(byteOffset);

				switch (type)
				{
				case MessageType::Connect:
				{
					ConnectData data;
					data.id = ReadInt(byteOffset);
					data.srcAddress = _packet->address;
					receiveData.SetData(data);
					break;
				}
				case MessageType::Disconnect:
				{
					DisconnectData data;
					data.srcAddress = _packet->address;
					receiveData.SetData(data);
					break;

				}
				case MessageType::State:
				{
					StateData data;
					data.id = ReadInt(byteOffset);
					data.xPos = ReadFloat(byteOffset);
					data.yPos = ReadFloat(byteOffset);
					data.xVel = ReadFloat(byteOffset);
					data.yVel = ReadFloat(byteOffset);
					data.srcAddress = _packet->address;
					receiveData.SetData(data);
					break;
				}
				}
			}
			return receiveData;
		}

	private:
		void WriteInt(int value);
		int ReadInt(int& byteOffset);
		void WriteFloat(float valueF);
		float ReadFloat(int & byteOffset);

		void WriteString(std::string& s);
		std::string ReadString(int& byteOffset);

		UDPpacket* _packet;
		UDPsocket _socket;
	};
}