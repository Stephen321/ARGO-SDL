#pragma once

#include "System.h"
#include "ParticleComponent.h"

class ParticleSystem : public System
{
public:
	ParticleSystem(float updateRate = 0.f);
	~ParticleSystem();

	void				Initialize(SDL_Renderer*& renderer);

	void				Process(float dt = 0.f) override;

private:
	SDL_Renderer*		_renderer;
};

