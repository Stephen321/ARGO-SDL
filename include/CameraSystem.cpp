#include "CameraSystem.h"

CameraSystem::CameraSystem()
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Process(float dt)
{
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		Camera2D::Vector2 centre;
		for (Entity* e : (*it).second)
		{
			BoundsComponent* bound = static_cast<BoundsComponent*>(e->GetComponent(Component::Type::Bounds));
			centre.x += bound->rect.x + bound->rect.w * 0.5f;
			centre.y += bound->rect.y + bound->rect.h * 0.5f;
		}
		if ((*it).second.empty() == false)
		{
			centre /= (*it).second.size();
		}
		_camera.setCentre(centre);
	}
}

void CameraSystem::Init(int w, int h)
{

	_camera.init(w, h);
}
