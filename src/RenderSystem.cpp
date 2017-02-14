#include "RenderSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

RenderSystem::RenderSystem(SDL_Renderer *& renderer, Camera2D::Camera* camera, float updateRate)
	: System(updateRate)
	, _renderer(renderer)
	, _camera(camera)
{
}

RenderSystem::~RenderSystem()
{
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
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

				SDL_Rect scaledRect = transform->rect;
				scaledRect.w *= transform->scaleX;
				scaledRect.h *= transform->scaleY;
				scaledRect.x -= transform->origin.x * transform->scaleX;
				scaledRect.y -= transform->origin.y * transform->scaleY;

				if (transform != nullptr && sprite != nullptr)
				{
					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &_camera->worldToScreen(scaledRect), transform->angle, &transform->origin, SDL_FLIP_NONE);
				}
			}
		}
	}
}
