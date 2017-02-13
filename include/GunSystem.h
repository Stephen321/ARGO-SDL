#pragma once

#include "System.h"


class GunSystem : public System
{
public:
				GunSystem(float updateRate = 0.f);
				~GunSystem();

	void		Process(float dt = 0.f) override;

	//GUN SYSTEM WHICH DEALS WITH GUN COMPONENT
};

