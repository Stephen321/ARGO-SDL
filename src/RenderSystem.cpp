#include "RenderSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "StatusEffectComponent.h"

RenderSystem::RenderSystem(float updateRate)
	: System(updateRate)
{
}

RenderSystem::~RenderSystem()
{
}


void RenderSystem::Initialize(SDL_Renderer*& renderer, Camera2D::Camera* camera)
{
	_renderer = renderer;
	_camera = camera;
}

void RenderSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				bool render = true;

				if (e->GetType() == EntityType::Player || e->GetType() == EntityType::AI)
				{
					StatusEffectComponent* statusEffect = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));

					if (statusEffect->invisible)
					{
						render = false;
					}
				}

				if (render)
				{
					TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
					SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &_camera->worldToScreen(transform->rect), transform->angle, &transform->origin, SDL_FLIP_NONE);
				}
			}
		}
	}
}
