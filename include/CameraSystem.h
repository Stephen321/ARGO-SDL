#pragma once
#include "Camera2D.h"
#include "SDL_Rect.h"
#include "System.h"
#include "BoundsComponent.h"


class CameraSystem : public System
{
public:
	CameraSystem();
	~CameraSystem();

	void Process(float dt = 0.f) override;
	Camera2D::Camera& getCamera() { return _camera; }
	void Init(int w, int h);

private:
	Camera2D::Camera	_camera;
};

