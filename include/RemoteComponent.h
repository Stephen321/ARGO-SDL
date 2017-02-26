#pragma once

#include "Component.h"


class RemoteComponent : public Component
{
public:
	RemoteComponent(int _id)
		: id(_id)
		, Component::Component(Component::Type::Remote)
	{
	}

	~RemoteComponent()
	{
	}

public:
	int id;
};