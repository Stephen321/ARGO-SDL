#include "RemoteSystem.h"
#include "PhysicsComponent.h"
#include "RemoteComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"

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
	_movementTimer += dt;
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
						ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
						//physics->xAcceleration = data.xAccel;
						//physics->yAcceleration = data.yAccel;
						//physics->xDir = data.xDir;
						//physics->yDir = data.yDir;
						physics->xVelocity = data.xVel;
						physics->yVelocity = data.yVel;
						collider->body->SetTransform(b2Vec2(data.xPos, data.yPos), collider->body->GetAngle());

						//Remote doesnt get the physics system doing this every physics system update: (!!! this is only done when received)....have another if statement that will transition between states with physics system update rate
						//------------------------------------------------------------------------------------------------------------------------------
						//PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
						TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
						//ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

						float maxVelocity = physics->maxVelocity;

						//float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
						//float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;

						//physics->xVelocity += (xDrag + (physics->xDir * physics->xAcceleration)) * dt;//change dt to _updateRate?//maybe?
						//physics->yVelocity += (yDrag + (physics->yDir * physics->yAcceleration)) * dt;

						if (physics->xVelocity + physics->yVelocity != 0)
						{
							transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
						}

						float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
						if (currentVelocity > maxVelocity)
						{
							physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
							physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
						}

						//if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
						//if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }

						collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

						transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
						transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);

						//if (e->GetType() == EntityType::Player)
						//{
						//	physics->xDir = 0;
						//	physics->yDir = 0;
						//}
						//------------------------------------------------------------------------------------------------------------------------------

					}
				}
			}
			break;
		}
		}
	}

	if (_movementTimer > PHYSICS_SYSTEM_UPDATE)
	{
		_movementTimer = 0.f;

		for (Entity* e : _entities[EntityType::RemotePlayer])
		{
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
			ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
			TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

			float maxVelocity = physics->maxVelocity;

			float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
			float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;

			physics->xVelocity += (xDrag + (physics->xDir * physics->xAcceleration)) * dt;//change dt to _updateRate?//maybe?
			physics->yVelocity += (yDrag + (physics->yDir * physics->yAcceleration)) * dt;

			if (physics->xVelocity + physics->yVelocity != 0)
			{
				transform->angle = atan2(physics->yVelocity, physics->xVelocity) * 180.f / M_PI;
			}

			float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
			if (currentVelocity > maxVelocity)
			{
				physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
				physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
			}

			if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.01f) { physics->xVelocity = 0.f; }
			if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.01f) { physics->yVelocity = 0.f; }

			collider->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

			transform->rect.x = (int)metersToPixels(collider->body->GetPosition().x);
			transform->rect.y = (int)metersToPixels(collider->body->GetPosition().y);
		}
	}
}
