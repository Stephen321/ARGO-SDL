#pragma once

#include "System.h"
#include "Camera2D.h"

#include "ControlComponent.h"
#include "FLInputManager.h"


class ControlSystem : public System, public EventListener
{
public:
					ControlSystem(Camera2D::Camera* camera, float updateRate = 0.f);
					~ControlSystem();

	void			Process(float dt = 0.f) override;

	void			MovePlayer(int _x, int _y, Entity*& entity);
	void			OnEvent(Event evt) override;

private:
	Camera2D::Camera*	_camera;
};

