#pragma once

#include "System.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "NetworkHandler.h"

class RemoteSystem : public System
{
public:
			RemoteSystem(float updateRate = 0.f);
			~RemoteSystem();

	void	Process(float dt = 0.f) override;
};

