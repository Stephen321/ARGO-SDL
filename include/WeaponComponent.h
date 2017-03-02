#pragma once

#include "Component.h"


class WeaponComponent : public Component
{
public:
	WeaponComponent()
		: id(-1)
		, hasWeapon(false)
		, fired(false)
		, angle(0.f)
		, Component::Component(Component::Type::Weapon)
	{
	}

	~WeaponComponent()
	{
	}

public:
	int id;
	float angle;
	bool fired;
	bool hasWeapon;
};
