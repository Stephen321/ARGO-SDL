#pragma once

#include "System.h"


class PhysicsSystem : public System
{
public:
	PhysicsSystem(float updateRate = 0.f);
	~PhysicsSystem();

	void Process(float dt = 0.f) override;
};

