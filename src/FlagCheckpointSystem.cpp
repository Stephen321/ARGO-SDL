#include "FlagCheckpointSystem.h"

#include "TransformComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"


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
			AddEntity(_interactionSystemEvents[FLAG_DROPPED].at(i).first, _interactionSystemEvents[FLAG_DROPPED].at(i).second);
		}

		_interactionSystemEvents[FLAG_DROPPED].clear();
	}
}