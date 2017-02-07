#pragma once

#include "SDL_render.h"
#include "System.h"
#include "BoundsComponent.h"
#include "SpriteComponent.h"


class RenderSystem : public System
{
public:
	RenderSystem(SDL_Renderer*& renderer);
	~RenderSystem();

	void Process(float dt = 0.f) override;

private:
	SDL_Renderer*& _renderer;
};

