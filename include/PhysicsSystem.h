#pragma once

#include "System.h"


class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Process(float dt = 0.f) override;
};

