#include "NetSystem.h"
#include "NetComponent.h"

NetSystem::NetSystem(float updateRate)
	: System(updateRate)
	, _net(5000)
{
}

NetSystem::~NetSystem()
{
}


void NetSystem::Initialize()
{
	SDLNet_ResolveHost(&_serverIp, "127.0.0.1", 5228);// 5228); //change to 5228 once ec2 is working
}

void NetSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				NetComponent* remote = static_cast<NetComponent*>(e->GetComponent(Component::Type::Net));
				int id = remote->id;
				if (id < 0 && remote->connecting == false) //hasnt tried to connect yet
				{
					ConnectData data;
					_net.Send(&data, _serverIp);
					remote->connecting = true;
				}
			}
		}
		
		ReceivedData receiveData = _net.Receive();
		if (receiveData.Empty() == false)
		{
			switch (receiveData.GetType())
			{
			case MessageType::Connect:
				ConnectData data = receiveData.GetData<ConnectData>();
				NetComponent* remote = static_cast<NetComponent*>(_entities[EntityType::Player].at(0)->GetComponent(Component::Type::Net));
				remote->id = data.id;
				std::cout << "ID is: " << remote->id << std::endl;
			}
		}
	}
}
