#pragma once

#include "System.h"
#include "FLInputManager.h"


class ControlSystem : public System, public EventListener
{
public:
	ControlSystem(float updateRate = 0.f);
	~ControlSystem();

	void Process(float dt = 0.f) override;

	void MovePlayer(int _x, int _y, Entity*& entity);

private:
};

