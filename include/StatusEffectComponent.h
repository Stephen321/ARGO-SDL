#pragma once

#include "Component.h"


class StatusEffectComponent : public Component
{
public:
	StatusEffectComponent()
		: Component::Component(Component::Type::StatusEffect)
		, staggered(false)
		, speedUp(false)
		, invisible(false)
		, invincible(false)
		, staggeredTimer(0)
		, speedUpTimer(0)
		, invisibleTimer(0)
		, invincibleTimer(0)
	{
		
	}

	~StatusEffectComponent()
	{
	}

public:
	bool	staggered;
	bool	speedUp;
	bool	invisible;
	bool	invincible;

	float	staggeredTimer;
	float	speedUpTimer;
	float	invisibleTimer;
	float	invincibleTimer;
};