#include "CameraSystem.h"

CameraSystem::CameraSystem(float updateRate)
	: System(updateRate)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		Camera2D::Vector2 centre;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				TransformComponent* bound = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				centre.x += bound->rect.x + bound->rect.w * 0.5f;
				centre.y += bound->rect.y + bound->rect.h * 0.5f;
			}
			if ((*it).second.empty() == false)
			{
				centre /= (*it).second.size();
			}
			float halfCamWidth = _camera.getSize().x * 0.5f;
			float halfCamHeight = _camera.getSize().y* 0.5f;

			if (centre.x < halfCamWidth)
				centre.x = halfCamWidth;
			else if (centre.x > WORLD_WIDTH - halfCamWidth)
				centre.x = WORLD_WIDTH - halfCamWidth;

			if (centre.y < halfCamHeight)
				centre.y = halfCamHeight;
			else if (centre.y > WORLD_HEIGHT - halfCamHeight)
				centre.y = WORLD_HEIGHT - halfCamHeight;
			_camera.setCentre(centre);
		}
	}
}

void CameraSystem::Init(int w, int h)
{

	_camera.init(w, h);
}
