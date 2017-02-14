#pragma once

#include "System.h"

#include "Camera2D.h"
#include "FLInputManager.h"


class ControlSystem : public System
{
public:
							ControlSystem(float updateRate = 0.f);
							~ControlSystem();

	void					Initialize(Camera2D::Camera* camera);

	void					Process(float dt = 0.f) override;

	void					AddTurret(Entity* entity);
	void					RemoveTurret(Entity* entity);



private:
	Camera2D::Camera*		_camera;
	std::vector<Entity*>	_turrets;

};

