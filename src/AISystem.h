#pragma once

#include "System.h"
#include "PhysicsSystem.h"
#include "BoundsComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"

class AISystem : public System
{
public:
	AISystem();
	~AISystem();

	void Process(float dt = 0.f) override;
};

