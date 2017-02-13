#pragma once

#include "System.h"

#include "Camera2D.h"
#include "FLInputManager.h"


class ControlSystem : public System, public EventListener
{
public:
							ControlSystem(Camera2D::Camera* camera, float updateRate = 0.f);
							~ControlSystem();

	void					Process(float dt = 0.f) override;

	void					MoveHorizontal(int dir, Entity*& entity);
	void					MoveVertical(int dir, Entity*& entity);

	void					FireBullet(Entity*& entity);

	void					OnEvent(Event evt) override;

	void					AddTurret(Entity* entity);
	void					RemoveTurret(Entity* entity);



private:
	Camera2D::Camera*		_camera;
	std::vector<Entity*>	_turrets;

};

