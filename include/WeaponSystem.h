#pragma once

#include "InteractionSystem.h"
#include "Camera2D.h"

class WeaponSystem : public InteractionSystem
{
public:
						WeaponSystem(float updateRate = 0.f);
						~WeaponSystem();

	void				Initialize(Camera2D::Camera* camera);

	void				Process(float dt = 0.f) override;

private:
	Camera2D::Camera*	_camera;
};

