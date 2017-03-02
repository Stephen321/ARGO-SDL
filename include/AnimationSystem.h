#pragma once

#include "System.h"

#include "ResourceIdentifier.h"

#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

#include "AnimationComponent.h"

class AnimationSystem : public System
{
public:
	AnimationSystem();
	~AnimationSystem();

	void Initialize();
	void Process(float dt = 0.f) override;

private:
	void FSM(AnimationComponent*& animation, Entity* e);

private:
	std::map<TextureID, SDL_Texture*>*	_textureHolder;
};

