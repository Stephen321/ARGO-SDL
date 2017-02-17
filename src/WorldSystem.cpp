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

				 int maxNode = _map->getNodesSize();
				 int flagNode = maxNode - 1;
				 GraphNode* node = _map->getNodes()[flagNode];
				 node->setPosition(helper::Vector2(transform->rect.x, transform->rect.y));
				 
				 bool hasNeighbourNode = false;
				 int closestNode = 0;
				 float closestDistance = 99999999.f;
				 int size = maxNode - 1; //-1 because the flag is the last node
				 for (int i = 0; i < size; i++)
				 {
					 float distance = (position - _map->getNodes()[i]->getPosition()).length();
					 if (distance < closestDistance)
					 {
						 closestDistance = distance;
						 closestNode = i;
					 }
					 if (distance < mapComponent->radius)
					 {
						hasNeighbourNode = true;
						GraphArc* arcFromFlag = _map->getArc(flagNode, i);
						if (arcFromFlag == nullptr)
						{
							float distanceBetweenNode = (_map->getNodes()[i]->getPosition() - _map->getNodes()[flagNode]->getPosition()).length();
							_map->addArc(flagNode, i, distanceBetweenNode, false);
						}
						else
						{

							helper::Vector2 flagPos = _map->getNodes()[flagNode]->getPosition();
							helper::Vector2 otherPos = _map->getNodes()[i]->getPosition();
							float distanceBetweenNode = (otherPos - flagPos).length();
							arcFromFlag->setWeight(distanceBetweenNode);
							arcFromFlag->setLine(flagPos, otherPos);

							GraphArc* arcToFlag = _map->getArc(i, flagNode);
							arcToFlag->setWeight(distanceBetweenNode);
							arcToFlag->setLine(flagPos, otherPos);
						}
						
					 }
					 else
					 {
						 //if (node->arcList().size() > 1)
						// {
							 GraphArc* arcFromFlag = _map->getArc(flagNode, i);
							 if (arcFromFlag != nullptr)
							 {
								 node->removeArc(_map->getNodes()[i]);
								 _map->getNodes()[i]->removeArc(node);
							 }	
						// }
					 }
				 }
				 if (!hasNeighbourNode)
				 {

				 }

			}
		}
	}
}
