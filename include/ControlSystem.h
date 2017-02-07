#pragma once

#include "System.h"
#include "ControlComponent.h"
#include "FLInputManager.h"


class ControlSystem : public System, public EventListener
{
public:
	ControlSystem();
	~ControlSystem();

	void Process(float dt = 0.f) override;

	void MovePlayer(int _x, int _y, Entity*& entity);

private:
};

