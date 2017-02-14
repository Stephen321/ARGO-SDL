#pragma once

#include "Entity.h"

class FunctionMaster
{
public:
									FunctionMaster() {}
									~FunctionMaster() {}

	void							MoveHorizontal(int dir, Entity*& entity);
	void							MoveVertical(int dir, Entity*& entity);

	void							FireBullet(Entity*& entity);

	void							RotateEntity(Entity* entity);
};