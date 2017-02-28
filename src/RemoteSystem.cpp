#include "RemoteSystem.h"
#include "PhysicsComponent.h"
#include "RemoteComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "Helpers.h"

RemoteSystem::RemoteSystem(float updateRate)
	: System(updateRate)
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
	if (dt > 5.f)
		return;
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

		ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));

		StateData data;
		data.xVel = physics->xVelocity;
		data.yVel = physics->yVelocity;
		data.xPos = collider->body->GetPosition().x;
		data.yPos = collider->body->GetPosition().y;

		network.Send(&data); //send player state to server every REMOTE_PACKET_RATE
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

			for (Entity* e : _entities[EntityType::RemotePlayer]) //only going to receive updates for remote players
			{
				RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
				if (remote->id == data.id) //we just received state data for this remote player
				{
					std::cout << "Received state data from " << data.id << std::endl;
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

					if (remote->timeSincePacket > _updateRate * 1.5f) //recover from packet loss, need to converge back to new position
					{
						std::cout << "got first packet after Packet loss, linear converge to where we will be (+ velocity)!" << std::endl;
						remote->startState.xPos = collider->body->GetPosition().x;
						remote->startState.yPos = collider->body->GetPosition().y;
						//remote->startState.r = rb.rotation; velocity?

						physics->xVelocity = data.xVel;
						physics->yVelocity = data.yVel;

						b2Vec2 target = b2Vec2(data.xPos, data.yPos) + (b2Vec2(physics->xVelocity * _updateRate, physics->yVelocity * _updateRate));
						remote->endState.xPos = target.x;
						remote->endState.yPos = target.y;
						//linearly converge to where we are plus the velocity so we move towards where we will be by the time we get there
					}

					//Just got a packet, reset timer for this remote player
					remote->timeSincePacket = 0.f;
					if (remote->startState == NULL) //if it is the first packet
					{
						std::cout << "got first packet" << std::endl;
						remote->startState.xPos = data.xPos;
						remote->startState.yPos = data.yPos;
						remote->startState.xVel = data.xVel;
						remote->startState.yVel = data.yVel;
						remote->endState = remote->startState;
					}
					else //every other packet
					{
						remote->startState = remote->endState; //if we havnt reached the endState then we will snap to it...this is if a message arrives early
						
						//start state is currently where the player is. this means within updateRate the player may 
						//not lerp fully to the endState as startState may be futher behind than remote actual state when the message was sent
						//remote->startState.xPos = collider->body->GetPosition().x;
						//remote->startState.yPos = collider->body->GetPosition().y;
						//remote->startState.xVel = remote->endState.xVel;
						//remote->startState.yVel = remote->endState.yVel;

						remote->endState.xPos = data.xPos;
						remote->endState.yPos = data.yPos;
						remote->endState.xVel = data.xVel;
						remote->endState.yVel = data.yVel;

						physics->xVelocity = data.xVel;
						physics->yVelocity = data.yVel;
					}
					break; //we've dealt with the data we received so we can break
				}
			}
			break;
		}
		}
	}

	for (Entity* e : _entities[EntityType::RemotePlayer])
	{
		RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
		ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
		TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));


		float maxVelocity = physics->maxVelocity;
		float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
		if (currentVelocity > maxVelocity)
		{
			physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
			physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
		}
		
		float percent = remote->timeSincePacket / _updateRate; //0 to 100%

		if (percent > 1.2f)//start extrapolating as the packet is late
		{
			//set velocity to the velocity we had in the last packet we received
			std::cout << "Start extrapolating" << std::endl;
			collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));
		}
		else //lerp between last packet and current packet receivedsss
		{
			b2Vec2 newPosition;
			newPosition.x = lerp(remote->startState.xPos, remote->endState.xPos, percent);
			newPosition.y = lerp(remote->startState.yPos, remote->endState.yPos, percent);
			collider->body->SetTransform(newPosition, collider->body->GetAngle());
		}
		transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
		transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);


		//calculate angle
		if (remote->startState.xVel + remote->startState.yVel != 0)
		{ //angle set to velocity (velocity is whatever the last packet we received said it was
			if (remote->endState.xVel + remote->endState.yVel != 0)
			{
				float startAngle = atan2(remote->startState.yVel, remote->startState.xVel) * 180.f / M_PI;
				if (startAngle < 0.f)
					startAngle += 360.f;
				float endAngle = atan2(remote->endState.yVel, remote->endState.xVel) * 180.f / M_PI;
				if (endAngle < 0.f)
					endAngle += 360.f;
				if (startAngle < 20.f && endAngle > 340.f)
					startAngle += 360.f;

				if (abs(endAngle - startAngle) > 180.f)
				{
					if (startAngle < endAngle)
					{
						startAngle += 360.f;
					}
					else if (endAngle < startAngle)
					{
						endAngle += 360.f;
					}
				}

				transform->angle = lerp(startAngle, endAngle, percent);
				std::cout << "start angle: " << startAngle << "  , " << endAngle << ".   " << transform->angle << std::endl;
			}
		}

		remote->timeSincePacket += dt; //time since last packet, between 0 and _updateRate
	}
}
