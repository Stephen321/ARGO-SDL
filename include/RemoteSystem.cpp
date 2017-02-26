#include "RemoteSystem.h"
#include "PhysicsComponent.h"
#include "RemoteComponent.h"

RemoteSystem::RemoteSystem(float updateRate)
{
}

RemoteSystem::~RemoteSystem()
{
}

void RemoteSystem::Initialize()
{
}

void RemoteSystem::Process(float dt)
{
	System::Process(dt);
	NetworkHandler& network = NetworkHandler::Instance();
	if (_canUpdate)
	{ //packet send rate (REMOTE_PACKET_RATE)
		_canUpdate = false;
		if (_entities.find(EntityType::Player) == _entities.end())
			return; //player wasnt added to remote system, therefore it isnt a multiplayer game
		Entity* player = _entities[EntityType::Player][0]; //only one player
		RemoteComponent* remote = static_cast<RemoteComponent*>(player->GetComponent(Component::Type::Remote));
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));

		StateData data;
		data.xAccel = physics->xAcceleration;
		data.yAccel = physics->yAcceleration;
		data.xDir = physics->xDir;
		data.yDir = physics->yDir;
		data.xVel = physics->xVelocity;
		data.yVel = physics->yVelocity;

		network.Send(&data); //send player state to server every REMOTE_PACKET_RATE (30 times a second default)
	}

	//always listening for messages

	ReceivedData receivedData = network.Receive();

	if (receivedData.Empty() == false)
	{

		switch (receivedData.GetType())
		{
		case MessageType::State:
		{
			StateData data = receivedData.GetData<StateData>();

			for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
			{
				for (Entity* e : (*it).second)
				{
					RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
					if (remote->id == data.id) //we just received state data for this remote player
					{
						std::cout << "Received state data from " << data.id << std::endl;
						PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
						physics->xAcceleration = data.xAccel;
						physics->yAcceleration = data.yAccel;
						physics->xDir = data.xDir;
						physics->yDir = data.yDir;
						physics->xVelocity = data.xVel;
						physics->yVelocity = data.yVel;

					}
				}
			}
			break;
		}
		}
	}

}
