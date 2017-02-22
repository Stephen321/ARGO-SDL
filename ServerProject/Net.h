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
		SessionList,
		JoinSession,
		SetHost
	};

	struct MessageData {
		float ts;
		int length;
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
		DisconnectData() : id(-1), sessionID(-1) { type = MessageType::Disconnect; }
		int id;
		int sessionID;
	};

	struct SessionListData : MessageData {
		SessionListData() { type = MessageType::SessionList; }
		int count;
		int maxPlayers;
		std::vector<int> sessionIDs;
		std::vector<int> currentPlayers;
	};

	struct JoinSessionData : MessageData {
		JoinSessionData() : id(-1), sessionID(-1) { type = MessageType::JoinSession; }
		int id;
		int sessionID;
	};

	struct SetHostData : MessageData {
		SetHostData() { type == MessageType::SetHost; }
	};

	//id in MessageData?
	//sessionId in MessageData?

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
				_data = new ConnectData(rhs.GetData<ConnectData>());
				break;
			}
			case MessageType::Disconnect:
			{
				_data = new DisconnectData(rhs.GetData<DisconnectData>());
				break;
			}
			case MessageType::State:
			{
				_data = new StateData(rhs.GetData<StateData>());
				break;
			}
			case MessageType::SessionList:
			{
				_data = new SessionListData(rhs.GetData<SessionListData>());
				break;
			}
			case MessageType::JoinSession:
			{
				_data = new JoinSessionData(rhs.GetData<JoinSessionData>());
				break;
			}
			case MessageType::SetHost:
			{
				_data = new SetHostData(rhs.GetData<SetHostData>());
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

		IPaddress GetSrcAddress() const
		{
			return _srcAddress;
		}

		void SetSrcAddress(const IPaddress& srcAddress)
		{
			_srcAddress = srcAddress;
		}

		template <typename T>
		T GetData() const
		{
			return *static_cast<T*>(_data);
		}

	private:
		IPaddress	_srcAddress;
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
				WriteInt(ddata->id);
				WriteInt(ddata->sessionID);
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
				WriteInt(jsdata->id);
				WriteInt(jsdata->sessionID);
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

		ReceivedData Receive()
		{
			ReceivedData receiveData;
			if (SDLNet_UDP_Recv(_socket, _packet) > 0)
			{
				receiveData.SetSrcAddress(_packet->address);
				int byteOffset = 0;
				MessageType type = (MessageType)ReadInt(byteOffset);

				switch (type)
				{
				case MessageType::Connect:
				{
					ConnectData data;
					data.id = ReadInt(byteOffset);
					receiveData.SetData(data);
					break;
				}
				case MessageType::Disconnect:
				{
					DisconnectData data;
					data.id = ReadInt(byteOffset);
					data.sessionID = ReadInt(byteOffset);
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
					receiveData.SetData(data);
					break;
				}
				case MessageType::SessionList:
				{
					SessionListData data;
					data.count = ReadInt(byteOffset);
					data.maxPlayers = ReadInt(byteOffset);
					for (int i = 0; i < data.count; i++)
					{ //is order the same?
						data.sessionIDs.push_back(ReadInt(byteOffset));
						data.currentPlayers.push_back(ReadInt(byteOffset));
					}
					receiveData.SetData(data);
					break;
				}
				case MessageType::JoinSession:
				{
					JoinSessionData data;
					data.id = ReadInt(byteOffset);
					data.sessionID = ReadInt(byteOffset);
					receiveData.SetData(data);
					break;
				}
				case MessageType::SetHost:
				{
					SetHostData data;
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
		std::string GetTypeAsString(MessageType type);

		UDPpacket* _packet;
		UDPsocket _socket;
	};
}