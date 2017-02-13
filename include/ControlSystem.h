#pragma once

#include "System.h"
#include "FLInputManager.h"


class ControlSystem : public System, public EventListener
{
public:
	ControlSystem(float updateRate = 0.f);
	~ControlSystem();

	void Process(float dt = 0.f) override;

	void MoveHorizontal(int dir, Entity*& entity);
	void MoveVertical(int dir, Entity*& entity);
};

