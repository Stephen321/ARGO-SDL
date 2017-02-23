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
		MessageData() : id(-1), sessionID(-1) {};
		int id;
		int sessionID;
		MessageType GetType() const {
			return type;
		}
	protected:
		MessageType type;
	};

	struct StateData : MessageData {
		StateData() { type = MessageType::State; }
		float xPos;
		float yPos;
		float xVel;
		float yVel;
	};
	struct ConnectData : MessageData {
		ConnectData() { type = MessageType::Connect; }
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

	struct JoinSessionData : MessageData {
		JoinSessionData() { type = MessageType::JoinSession; }
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
		void Send(MessageData* data, const char * destHost, int destPort);
		void Send(MessageData* data, IPaddress destAddr);
		ReceivedData Receive();

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