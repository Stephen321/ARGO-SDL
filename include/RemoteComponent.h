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
	struct State {
		float xPos;
		float yPos;
		float xVel;
		float yVel;
		float ts;
	};
	State startState;
	State endState;
	int id;
};