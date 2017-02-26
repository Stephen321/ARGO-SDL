#pragma once

#include "Component.h"


class WeaponComponent : public Component
{
public:
	WeaponComponent()
		: id(-1)
		, hasWeapon(false)
		, Component::Component(Component::Type::Weapon)
	{
	}

	~WeaponComponent()
	{
	}

public:
	int id;

	bool hasWeapon;
};
