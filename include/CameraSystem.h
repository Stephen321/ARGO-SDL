#pragma once
#include "Camera2D.h"
#include "SDL_Rect.h"
#include "System.h"
#include "BoundsComponent.h"
#include "ConstHolder.h"


class CameraSystem : public System
{
public:
	CameraSystem(float updateRate = 0.f);
	~CameraSystem();

	void Process(float dt = 0.f) override;
	Camera2D::Camera& getCamera() { return _camera; }
	void Init(int w, int h);

private:
	Camera2D::Camera	_camera;
};

