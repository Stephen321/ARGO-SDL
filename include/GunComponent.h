#pragma once

#include "Component.h"


class GunComponent : public Component
{
public:
	GunComponent(float mt, int id)
		: maxTimer(mt)
		, ammo(0)
		, fireTimer(0)
		, canFire(true)
		, triggered(false)
		, owner(EntityType::AI)
		, Component::Component(Component::Type::Gun)
	{
	}

	GunComponent(float mt, int a, int id)
		: maxTimer(mt)
		, ammo(a)
		, fireTimer(0)
		, canFire(true)
		, triggered(false)
		, owner(EntityType::AI)
		, Component::Component(Component::Type::Gun)
	{
	}

	~GunComponent()
	{
	}

public:
	int ammo;
	int id;

	float fireTimer;
	float maxTimer;

	bool canFire;
	bool triggered;
	EntityType owner;
};