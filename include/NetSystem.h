#pragma once

#include "System.h"
#include "Net.h"

using namespace Network;

class NetSystem : public System
{
public:
	NetSystem(float updateRate = 0.f);
	~NetSystem();

	void					Initialize();

	void					Process(float dt = 0.f) override;

private:
	Net _net;
	IPaddress _serverIp;
};