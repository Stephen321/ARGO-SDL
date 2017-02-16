#include "WorldSystem.h"
#include "AIComponent.h"
#include "TransformComponent.h" 
#include "MapComponent.h"

WorldSystem::WorldSystem(float updateRate)
	: System(updateRate)
{
}

WorldSystem::~WorldSystem()
{

}

void WorldSystem::Initialize(Graph* map)
{
	_map = map;
}

void WorldSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				 MapComponent* mapComponent = static_cast<MapComponent*>(e->GetComponent(Component::Type::Map));
				 TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
				 helper::Vector2 position = helper::Vector2(transform->rect.x, transform->rect.y);
				 int maxNode = _map->geNodesSize();

				 for (int i = 0; i < maxNode; i++)
				 {
					 float distance = (position - _map->nodeArray()[i]->getPosition()).length();
					 if (distance < mapComponent->radius)
					 {
						 //_map->addNode("", n);
						 //_map->addNode("Player", maxNode,  )
					 }
				 }

			}
		}
	}
}
