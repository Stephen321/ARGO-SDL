#pragma once

#include "Component.h"


class GunComponent : public Component
{
public:
	GunComponent(float mt)
		: maxTimer(mt)
		, ammo(0)
		, fireTimer(0)
		, canFire(true)
		, triggered(false)
		, isPlayers(false)
		, Component::Component(Component::Type::Gun)
	{
	}

	GunComponent(float mt, int a)
		: maxTimer(mt)
		, ammo(a)
		, fireTimer(0)
		, canFire(true)
		, triggered(false)
		, isPlayers(false)
		, Component::Component(Component::Type::Gun)
	{
	}

	~GunComponent()
	{
	}

public:
	int ammo;

	float fireTimer;
	float maxTimer;

	bool canFire;
	bool triggered;
	bool isPlayers;
};