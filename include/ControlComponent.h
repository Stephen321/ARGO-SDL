#pragma once

#include "Component.h"


class ControlComponent : public Component
{
public:
	ControlComponent()
		: Component::Component(Component::Type::Control)
	{
	}

	~ControlComponent()
	{
	}

public:
	//TBD
};
