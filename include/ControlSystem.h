#pragma once

#include "System.h"


class ControlSystem : public System
{
public:
	ControlSystem();
	~ControlSystem();

	void Process(float dt = 0.f) override;
};

