#pragma once
#include "Camera2D.h"
#include "SDL_render.h"
#include "System.h"
#include "BoundsComponent.h"
#include "SpriteComponent.h"


class RenderSystem : public System
{
public:
	RenderSystem(SDL_Renderer*& renderer, Camera2D::Camera* camera);
	~RenderSystem();

	void Process(float dt = 0.f) override;

private:
	Camera2D::Camera*	_camera;
	SDL_Renderer*&		_renderer;
};

