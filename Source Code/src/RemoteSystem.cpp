#include "RemoteSystem.h"
#include "PhysicsComponent.h"
#include "RemoteComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "Helpers.h"
#include "WeaponComponent.h"
#include "FlagComponent.h"
#include "DestructionComponent.h"
#include "StatusEffectComponent.h"

RemoteSystem::RemoteSystem(float updateRate, std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests)
	: System(updateRate)
	, _creationRequests(creationRequests)
	, _flagHolderID(-1)
	, _pinging(true)
	, _pingTimer(0.f)
{
}

RemoteSystem::~RemoteSystem()
{
}

void RemoteSystem::Initialize(Graph* waypoints, SystemManager* systemManager)
{
	_waypoints = waypoints;
	_systemManager = systemManager;
}

void RemoteSystem::Process(float dt)
{
	if (dt > 5.f)
		return;
	System::Process(dt);



	NetworkHandler& network = NetworkHandler::Instance();

	_pingTimer += dt;
	if (_pinging)
	{
		if (_pingTimer > PING_SEND_RATE)
		{
			PingData ping;
			network.Send(&ping);
			_pingTimer = 0.f;
		}
	}
	else
	{
		if (_pingTimer > SYNC_RATE)
		{
			_pinging = true;
			_pingTimer = 0.f;
		}
	}

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
			data.xPos = collider->body->GetPosition().x + data.xVel * 0.06f;
			data.yPos = collider->body->GetPosition().y + data.yVel * 0.06f;
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
			case MessageType::Ping:
			{
				PingData data = receivedData.GetData<PingData>();
				float rtt = network.gameTime - data.ts;
				float serverTimeDelta = (data.serverTime - network.gameTime) + (rtt * 0.5f);
				if (_serverDeltas.empty()) //update gametime the first time
					network.gameTime += serverTimeDelta;
				_serverDeltas.push_back(serverTimeDelta);
				if (_serverDeltas.size() > PING_SEND_COUNT)
				{
					_serverDeltas.erase(_serverDeltas.begin());
					_pinging = false;
					//take median
					float median;
					int size = _serverDeltas.size();

					sort(_serverDeltas.begin(), _serverDeltas.end());

					if (size % 2 == 0)
					{
						median = (_serverDeltas[size / 2 - 1] + _serverDeltas[size / 2]) / 2;
					}
					else
					{
						median = _serverDeltas[size / 2];
					}
					network.serverTimeDelta = median;
					network.gameTime += network.serverTimeDelta;
					_pingTimer = 0.f;
				}
				std::cout << "GameTime: " << data.ts << ", serverTime: " << data.serverTime << std::endl;
				break;
			}
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

							RemoteComponent::State state;
							state.ts = data.ts;
							state.xPos = data.xPos;
							state.yPos = data.yPos;
							state.xVel = data.xVel;
							state.yVel = data.yVel;
							if (remote->states.empty() == false)
							{
								if (state.ts < remote->states.back().ts)
								{//straggler so ignore
									std::cout << "straggler!!!!" << std::endl;
									break;
								}
							}
							remote->states.push(state);
							if (remote->states.size() > 5)
								remote->states.pop();
							if (remote->startState == NULL) //if it is the first packet
							{
								std::cout << "got first packet" << std::endl;
								remote->startState = state;
								remote->endState = remote->startState;
							}
							else //every other packet
							{
								remote->startState = remote->endState; //if we havnt reached the endState then we will snap to it...this is if a message arrives early

								remote->endState = state;
							}

							if (remote->timeSincePacket > _updateRate * 2.f) //recover from packet loss, need to converge back to new position
							{
								remote->startState.xPos = collider->body->GetPosition().x;
								remote->startState.yPos = collider->body->GetPosition().y;

								b2Vec2 target = b2Vec2(data.xPos, data.yPos);// +(b2Vec2(physics->xVelocity * _updateRate, physics->yVelocity * _updateRate));
								remote->endState.xPos = target.x;
								remote->endState.yPos = target.y;
							}

							//Just got a packet, reset timer for this entity
							remote->timeSincePacket = 0.f;

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
				//stagger player
				bool staggered = false;
				for (EntityMapIterator it = _entities.begin(); it != _entities.end() && staggered == false; ++it)
				{
					for (Entity* e : it->second)
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
						if (data.remoteID == remote->id)
						{
							StatusEffectComponent* playerStatusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));
							playerStatusEffects->staggered = true;
							playerStatusEffects->staggeredTimer = STAGGER_MAX_TIMER;
							staggered = true;
							break;
						}
					}

				}
				_flagHolderID = -1;
				break;
			}
			case MessageType::Invis:
			{
				InvisData data = receivedData.GetData<InvisData>();
				
				bool invised = false;
				for (EntityMapIterator it = _entities.begin(); it != _entities.end() && invised == false; ++it)
				{
					for (Entity* e : it->second)
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
						if (data.remoteID == remote->id)
						{
							StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));
							statusEffects->invisible = true;
							statusEffects->invisibleTimer = INVISIBLE_MAX_TIMER;
							invised = true;
							break;
						}
					}

				}
				_flagHolderID = -1;
				break;
			}
			case MessageType::CheckConnection:
			{
				CheckConnectionData data = receivedData.GetData<CheckConnectionData>();
				network.Send(&data);
				break;
			}
			case MessageType::Disconnect:
			{
				DisconnectData data = receivedData.GetData<DisconnectData>();

				if (data.id == NetworkHandler::Instance().GetPlayerID())
				{
					break;
				}
				std::cout << data.id << " disconnected." << std::endl;
				for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
				{
					for (Entity* e : it->second)
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote)); 
						if (data.id == remote->id)
						{ //found the player which disconnected 

							Entity* flag = _entities[EntityType::Flag][0];
							//check if it had the flag first!!!!!

							ColliderComponent* colliderFlag = static_cast<ColliderComponent*>(flag->GetComponent(Component::Type::Collider));
							TransformComponent* transformFlag = static_cast<TransformComponent*>(flag->GetComponent(Component::Type::Transform));
							colliderFlag->setActive = true;
							transformFlag->rect.x = (int)metersToPixels(colliderFlag->body->GetPosition().x);
							transformFlag->rect.y = (int)metersToPixels(colliderFlag->body->GetPosition().y);
							//_flagHolderID = -1;

							//...replace with ai???...
							DestructionComponent* destroy = static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy));
							//destroy->destroy = true;			 //doesnt remote it from all systems and delete components
							

							FlagComponent* flagComp = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
							if (flagComp != nullptr)
								flagComp->hasFlag = false;
							//move offscreen...doesnt work either..
							std::cout << "moving disconnected player" << std::endl;
							remote->disconnected = true;
							//temp move off the map?? (what happens if it has flag)
							ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
							collider->body->SetTransform(b2Vec2(-10000, -10000), 0.f);
							collider->setActive = false;
							TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
							transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
							transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
						}
						
					}
				}
				break;
			}
			case MessageType::SetHost:
			{
				std::cout << "Becoming new host" << std::endl;
				SetHostData data = receivedData.GetData<SetHostData>();
				network.SetHost(true);

				bool findFlagHolder = true;
				Entity* flag = _entities[EntityType::Flag][0];
				ColliderComponent* collider = static_cast<ColliderComponent*>(flag->GetComponent(Component::Type::Collider));
				collider->setActive = true;
				for (EntityMapIterator it = _entities.begin(); it != _entities.end() && findFlagHolder; ++it)
				{
					for (Entity* e : it->second)
					{
						RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));
						if (remote->id == _flagHolderID)
						{
							FlagComponent* flagComp = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
							flagComp->hasFlag = true;
							findFlagHolder = false;
							//_flagHolderID = -1;

							//TransformComponent* transform = static_cast<TransformComponent*>(flag->GetComponent(Component::Type::Transform));
						/*	b2Vec2 p;
							p.x = (int)pixelsToMeters(transform->rect.x);
							p.y = (int)pixelsToMeters(transform->rect.y);
							collider->body->SetTransform(p, collider->body->GetAngle());*/
							break;
						}
					}
				}
				//host migration
				for (Entity* e : _entities[EntityType::AI])
				{
					_systemManager->AddEntity(SystemType::Physics, e);
					_systemManager->AddEntity(SystemType::Animation, e);
				}
				for (Entity* e : _entities[EntityType::Flag])
				{
					_systemManager->AddEntity(SystemType::Physics, e);
				}
				//what else to do??
				break;
			}
		}
	}

	if (_flagHolderID != -1 && NetworkHandler::Instance().GetHost() == false)
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
			RemoteComponent* remote = static_cast<RemoteComponent*>(e->GetComponent(Component::Type::Remote));

			if (remote->disconnected && e->GetType() == EntityType::Player || (e->GetType() == EntityType::Flag && _flagHolderID != -1)) //don't do any lerping for the local player
			{
				break;
			}

			if (remote->startState == NULL) //dont attempt to lerp if no packets where received for this entity yet
			{
				break;
			}

			PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
			ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
			TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
			
			//if (e->GetType() == EntityType::RemotePlayer)
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

			collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

			if (percent < 1.f)//lerp between last packet and current packet receivedsss
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
