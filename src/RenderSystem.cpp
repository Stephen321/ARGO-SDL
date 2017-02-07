#include "RenderSystem.h"


RenderSystem::RenderSystem(SDL_Renderer *& renderer)
	: _renderer(renderer)
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Process(float dt)
{
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* e : (*it).second)
		{
			BoundsComponent* bound = static_cast<BoundsComponent*>(e->GetComponent(Component::Type::Bounds));
			SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));
			SDL_RenderCopy(_renderer, sprite->texture, &sprite->sourceRect, &bound->rect);
		}
	}
}
