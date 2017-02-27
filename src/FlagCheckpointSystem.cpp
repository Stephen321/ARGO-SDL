#include "FlagCheckpointSystem.h"

#include "TransformComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

#include "Helpers.h"


FlagCheckpointSystem::FlagCheckpointSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, float updateRate)
	: InteractionSystem(updateRate)
	, _interactionSystemEvents(events)
	, FLAG_PICKED(InteractionSystemEvent::FlagPicked)
	, FLAG_DROPPED(InteractionSystemEvent::FlagDropped)
{	  
}


FlagCheckpointSystem::~FlagCheckpointSystem()
{
}



void FlagCheckpointSystem::Initialize(std::vector<Entity*>& checkpoints)
{
	_checkpoints = checkpoints;
}

void FlagCheckpointSystem::Process(float dt)
{
	InteractionSystem::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;

		ListenForEvents();

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			FlagComponent* flag = static_cast<FlagComponent*>(it->first->GetComponent(Component::Type::Flag));

			TransformComponent* transform1 = static_cast<TransformComponent*>(it->first->GetComponent(Component::Type::Transform));
			TransformComponent* transform2 = static_cast<TransformComponent*>(it->second->GetComponent(Component::Type::Transform));

			transform2->rect.x = transform1->rect.x;
			transform2->rect.y = transform1->rect.y;

			for (int i = 0; i < _checkpoints.size(); i++)
			{
				CheckpointComponent* checkpoint = static_cast<CheckpointComponent*>(_checkpoints.at(i)->GetComponent(Component::Type::Checkpoint));

				checkpoint->highlighted = (flag->currentCheckpointID + 1 == i);
			}
		}
	}
}

void FlagCheckpointSystem::ListenForEvents()
{
	FlagPickedEvent();
	FlagDroppedEvent();
}
void FlagCheckpointSystem::FlagPickedEvent()
{
	if (!_interactionSystemEvents[FLAG_PICKED].empty())
	{
		for (int i = 0; i < _interactionSystemEvents[FLAG_PICKED].size(); i++)
		{
			AddEntity(_interactionSystemEvents[FLAG_PICKED].at(i).first, _interactionSystemEvents[FLAG_PICKED].at(i).second);
		}

		_interactionSystemEvents[FLAG_PICKED].clear();
	}
}
void FlagCheckpointSystem::FlagDroppedEvent()
{
	if (!_interactionSystemEvents[FLAG_DROPPED].empty())
	{
		for (int i = 0; i < _interactionSystemEvents[FLAG_DROPPED].size(); i++)
		{
			for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
			{
				PhysicsComponent* ownerPhysics = static_cast<PhysicsComponent*>(_interactionSystemEvents[FLAG_DROPPED].at(i).first->GetComponent(Component::Type::Physics));
				PhysicsComponent* characterPhysics = static_cast<PhysicsComponent*>(_interactionSystemEvents[FLAG_DROPPED].at(i).second->GetComponent(Component::Type::Physics));
				PhysicsComponent* flagPhysics = static_cast<PhysicsComponent*>(it->second->GetComponent(Component::Type::Physics));

				float ownerSpeed = sqrt(ownerPhysics->xVelocity * ownerPhysics->xVelocity + ownerPhysics->yVelocity * ownerPhysics->yVelocity);
				float characterSpeed = sqrt(characterPhysics->xVelocity * characterPhysics->xVelocity + characterPhysics->yVelocity * characterPhysics->yVelocity);

				float xDir = rand() % 20000 - 10000;
				float yDir = rand() % 20000 - 10000;

				float directionLenght = sqrt(xDir * xDir + yDir * yDir);

				flagPhysics->xVelocity = (xDir / directionLenght) * (ownerSpeed + characterSpeed);
				flagPhysics->yVelocity = (yDir / directionLenght) * (ownerSpeed + characterSpeed);

				if (flagPhysics->xVelocity > flagPhysics->maxVelocity)
				{
					flagPhysics->xVelocity = flagPhysics->maxVelocity;
				}
				else if (flagPhysics->xVelocity < -flagPhysics->maxVelocity)
				{
					flagPhysics->xVelocity = -flagPhysics->maxVelocity;
				}

				if (flagPhysics->yVelocity > flagPhysics->maxVelocity)
				{
					flagPhysics->yVelocity = flagPhysics->maxVelocity;
				}
				else if (flagPhysics->yVelocity < -flagPhysics->maxVelocity)
				{
					flagPhysics->yVelocity = -flagPhysics->maxVelocity;
				}

				FlagComponent* flag = static_cast<FlagComponent*>(it->first->GetComponent(Component::Type::Flag));
				flag->hasFlag = false;

				TransformComponent* actorTransform = static_cast<TransformComponent*>(it->first->GetComponent(Component::Type::Transform));
				ColliderComponent* collider = static_cast<ColliderComponent*>(it->second->GetComponent(Component::Type::Collider));

				collider->setActive = true;
				collider->body->SetTransform(b2Vec2(pixelsToMeters(actorTransform->rect.x), pixelsToMeters(actorTransform->rect.y)), 0);
				static_cast<CheckpointComponent*>(_checkpoints.at(flag->currentCheckpointID)->GetComponent(Component::Type::Checkpoint))->highlighted = false;
			}

			RemoveEntity(_interactionSystemEvents[FLAG_DROPPED].at(i).first, true);
		}

		_interactionSystemEvents[FLAG_DROPPED].clear();
	}
}