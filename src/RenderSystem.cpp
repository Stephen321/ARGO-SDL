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
				TransformComponent* bound = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				SpriteComponent* sprite = static_cast<SpriteComponent*>(e->GetComponent(Component::Type::Sprite));

				SDL_Rect scaledRect = bound->rect;
				scaledRect.w *= bound->scaleX;
				scaledRect.h *= bound->scaleY;
				scaledRect.x -= bound->origin.x * bound->scaleX;
				scaledRect.y -= bound->origin.y * bound->scaleY;

				if (bound != nullptr && sprite != nullptr)
				{
					//Render to screen
					SDL_RenderCopyEx(_renderer, sprite->texture, &sprite->sourceRect, &_camera->worldToScreen(scaledRect), bound->angle, &bound->origin, SDL_FLIP_NONE);
				}
			}
		}
	}
}
