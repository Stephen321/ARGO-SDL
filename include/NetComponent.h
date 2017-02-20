#pragma once

#include "Component.h"

class NetComponent : public Component
{
public:
	NetComponent()
		: Component::Component(Component::Type::Net)
		, id(-1)
		, connecting(false)
	{
	}

	~NetComponent()
	{
	}

public:
	int id;
	bool connecting;
};
