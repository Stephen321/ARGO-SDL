#pragma once

#include "Component.h"


class HealthComponent : public Component
{
public:
	HealthComponent(int h, int mh, bool a)
		: health(h)
		, maxHealth(mh)
		, alive(a)
		, Component::Component(Component::Type::Health)
	{
	}

	~HealthComponent()
	{
	}

public:
	int		health;
	int		maxHealth;
	bool	alive;
};

