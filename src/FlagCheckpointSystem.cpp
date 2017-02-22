#include "FlagCheckpointSystem.h"

#include "TransformComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"


FlagCheckpointSystem::FlagCheckpointSystem(float updateRate)
	: InteractionSystem(updateRate)
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