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

	//cant have these here due to the way moveplayer is being called by inputmanager (nullptr)
//private:
//	int _prevX;
//	int _prevY;
};

