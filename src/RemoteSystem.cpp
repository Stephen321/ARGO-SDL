#include "RemoteSystem.h"
#include "PhysicsComponent.h"
#include "RemoteComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "Helpers.h"
#include "WeaponComponent.h"
#include "FlagComponent.h"

RemoteSystem::RemoteSystem(float updateRate, std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests)
	: System(updateRate)
	, _creationRequests(creationRequests)
	, _flagHolderID(-1)
{
}

RemoteSystem::~RemoteSystem()
{
}

void RemoteSystem::Initialize(Graph* waypoints)
{
	_waypoints = waypoints;
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

		StateData data;
		if (NetworkHandler::Instance().GetHost())//host sends all state data for all remote/local/ai players and bullets
		{
			for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
			{
				for (Entity* e : it->second)
				{
					if (e->GetType() == EntityType::Flag && _flagHolderID != -1) 
					{
						break;
					}
					RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

					WeaponComponent* weapon = static_cast<WeaponComponent*>(e->GetComponent(Component::Type::Weapon));

					data.xVel = physics->xVelocity;
					data.yVel = physics->yVelocity;
					data.xPos = collider->body->GetPosition().x;
					data.yPos = collider->body->GetPosition().y;
					data.host = true;
					data.remoteID = remote->id;
					//	//find which entity has the flag
					//	for (EntityMapIterator it2 = _entities.begin(); it2 != _entities.end(); ++it2)
					//	{
					//		for (Entity* e2 : it2->second)
					//		{
					//			FlagComponent* flag = static_cast<FlagComponent*>(e2->GetComponent(Component::Type::Flag));
					//			if (flag != nullptr && flag->hasFlag)
					//			{//player has a flag so flag collider pos is not correct. need to find which player has it and set flag pos to there
					//				ColliderComponent* collider2 = static_cast<ColliderComponent*>(e2->GetComponent(Component::Type::Collider));
					//				data.xPos = collider2->body->GetPosition().x;
					//				data.yPos = collider2->body->GetPosition().y;
					//				break;
					//			}
					//		}
					//	}
					//	std::cout << "Host sending flag data " << remote->id << ". with xVel " << data.xVel << " yVel " << data.yVel << ".xPos " << data.xPos << ".yPos " << data.yPos << std::endl;
					//std::cout << "@@@Host sends player " << remote->id << " with xVel " << data.xVel << " yVel " << data.yVel << " . xPos " << data.xPos << " . yPos " << data.yPos << std::endl;
					network.Send(&data);
				}
			}
		}
		else //not the host so only send our local player
		{
			Entity* player = _entities[EntityType::Player][0]; //only one player
			RemoteComponent* remote = static_cast<RemoteComponent*>(player->GetComponent(Component::Type::Remote));
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));

			ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));

			WeaponComponent* weapon = static_cast<WeaponComponent*>(player->GetComponent(Component::Type::Weapon));

			data.xVel = physics->xVelocity;
			data.yVel = physics->yVelocity;
			data.xPos = collider->body->GetPosition().x;
			data.yPos = collider->body->GetPosition().y;
			data.host = false;
			data.remoteID = remote->id;
			network.Send(&data);
		}
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

				if (data.remoteID == NetworkHandler::Instance().GetPlayerID()) //ignore packets trying to update our local player
				{
					break;
				}
				for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
				{
					for (Entity* e : it->second)  //received state data
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
						if (remote->id == data.remoteID) //we just received state data for this entity
						{
							//std::cout << "other receives" << remote->id << " with xVel " << data.xVel << " yVel " << data.yVel << " . xPos " << data.xPos << " . yPos " << data.yPos << std::endl;	
							PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
							ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

							physics->xVelocity = data.xVel;
							physics->yVelocity = data.yVel;

							bool converge = false;
							if (remote->timeSincePacket > _updateRate * 2.f) //recover from packet loss, need to converge back to new position
							{
								converge = true;
								remote->startState.xPos = collider->body->GetPosition().x;
								remote->startState.yPos = collider->body->GetPosition().y;

								b2Vec2 target = b2Vec2(data.xPos, data.yPos);// +(b2Vec2(physics->xVelocity * _updateRate, physics->yVelocity * _updateRate));
								remote->endState.xPos = target.x;
								remote->endState.yPos = target.y;
							}

							//Just got a packet, reset timer for this entity
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
							else if (converge == false)//every other packet
							{
								remote->startState = remote->endState; //if we havnt reached the endState then we will snap to it...this is if a message arrives early

								remote->endState.xPos = data.xPos;
								remote->endState.yPos = data.yPos;
								remote->endState.xVel = data.xVel;
								remote->endState.yVel = data.yVel;
							}

							collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));
							break; //we've dealt with the data we received so we can break
						}
					}
				}
				break;
			}
			case MessageType::CreatePowerUp:
			{
				CreatePowerUpData data = receivedData.GetData<CreatePowerUpData>();

				std::vector<float> powerData = std::vector<float>();

				powerData.push_back(data.powerType);
				powerData.push_back(_waypoints->getNodes()[data.index]->getPosition().x); //xPosition
				powerData.push_back(_waypoints->getNodes()[data.index]->getPosition().y); //yPosition
				powerData.push_back(data.index); //node index

				_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::PowerUp, powerData));
				_waypoints->getNodes()[data.index]->setData(make_pair(GraphNode::EntityData::PowerUp, data.powerType));
				break;
			}
			case MessageType::Fire:
			{
				FireData data = receivedData.GetData<FireData>();

				for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
				{
					for (Entity* e : it->second)
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
						if (data.id == remote->id)
						{//this is the player that fired
							WeaponComponent* weapon = static_cast<WeaponComponent*>(e->GetComponent(Component::Type::Weapon));
							weapon->fired = true;
						}
					}

				}
				break;
			}
			case MessageType::PickUpFlag:
			{
				PickUpFlagData data = receivedData.GetData<PickUpFlagData>();

				for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
				{
					for (Entity* e : it->second)
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
						if (data.remoteID == remote->id)
						{ 
							_flagHolderID = data.remoteID;
						}
					}

				}
				break;
			}
			case MessageType::DroppedFlag:
			{
				DroppedFlagData data = receivedData.GetData<DroppedFlagData>();
				//stagger here
				//first check if invincable
				_flagHolderID = -1;
				break;
			}
		}
	}

	if (_flagHolderID != -1)
	{
		Entity* flag = _entities[EntityType::Flag][0];
		bool findFlagHolder = true;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end() && findFlagHolder; ++it)
		{
			for (Entity* e : it->second)
			{
				RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
				ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
				if (remote->id == _flagHolderID)
				{
					TransformComponent* transform = static_cast<TransformComponent*>(flag->GetComponent(Component::Type::Transform));
					transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
					transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
					findFlagHolder = false;
					break;
				}
			}
		}
	}
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* e : it->second)
		{
			if (e->GetType() == EntityType::Player || (e->GetType() == EntityType::Flag && _flagHolderID != -1)) //don't do any lerping for the local player
			{
				break;
			}
			RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));

			if (remote->startState == NULL) //dont attempt to lerp if no packets where received for this entity yet
			{
				break;
			}

			PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
			ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
			TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

			//if (e->GetType() == EntityType::Remote)
			//{
			//	std::cout << "remote velocity: " << physics->xVelocity << " , y: " << physics->yVelocity << std::endl;
			//}


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


			//calculate angle lerped between start and end state velocities
			if (e->GetType() != EntityType::Flag && remote->startState.xVel + remote->startState.yVel != 0)
			{
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
				}
			}

			remote->timeSincePacket += dt; //time since last packet, between 0 and _updateRate
		}
	}
}
