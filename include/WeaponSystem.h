#pragma once

#include "InteractionSystem.h"


class WeaponSystem : public InteractionSystem
{
public:
						WeaponSystem(float updateRate = 0.f);
						~WeaponSystem();

	void				Process(float dt = 0.f) override;
};

