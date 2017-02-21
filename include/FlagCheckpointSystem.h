#pragma once
#include "InteractionSystem.h"
class FlagCheckpointSystem : public InteractionSystem
{
public:
						FlagCheckpointSystem(float updateRate = 0.f);
						~FlagCheckpointSystem();

	void				Initialize(std::vector<Entity*>& checkpoints);

	void				Process(float dt = 0.f) override;

private:
	std::vector<Entity*> _checkpoints;
};

