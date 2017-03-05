#pragma once
#include <queue>
#include "Component.h"

class RemoteComponent : public Component
{
public:
	struct State 
	{
		State() : xPos(-100000.f), yPos(-100000.f), xVel(0.f), yVel(0.f), ts(0.f) {}
		float xPos;
		float yPos;
		float xVel;
		float yVel;
		float ts;
		bool operator==(int num) //if comparing to null, return true if xpos and ypos havnt been set
		{
			if (num == 0)
				return (xPos == -100000.f && yPos == -100000.f);
			else
				return false;
		}
	};

public:
	RemoteComponent(int _id)
		: id(_id)
		, Component::Component(Component::Type::Remote)
		, disconnected(false)
	{
	}

	~RemoteComponent()
	{
	}

public:
	State	startState;
	State	endState;
	std::queue<State> states;
	int		id;
	float	timeSincePacket;
	bool	disconnected;
};