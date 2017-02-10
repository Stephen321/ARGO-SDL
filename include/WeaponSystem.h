#pragma once

#include "InteractionSystem.h"


class WeaponSystem : public InteractionSystem
{
public:
						WeaponSystem();
						~WeaponSystem();

	void				Process(float dt = 0.f) override;
};

