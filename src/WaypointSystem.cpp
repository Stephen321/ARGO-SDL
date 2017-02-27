#include "WaypointSystem.h"
#include "TransformComponent.h" 
#include "ColliderComponent.h"
#include "Helpers.h"
WaypointSystem::WaypointSystem(float updateRate)
	: System(updateRate)
{
}

WaypointSystem::~WaypointSystem()
{

}

void WaypointSystem::Initialize(Graph* waypoints)
{
	_waypoints = waypoints;
}

void WaypointSystem::Process(float dt)
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
				ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));
				helper::Vector2 position = helper::Vector2(transform->rect.x, transform->rect.y);

				int maxNode = _waypoints->getNodesSize();
				int flagNode = maxNode - 1;
				GraphNode* node = _waypoints->getNodes()[flagNode];
				node->setPosition(helper::Vector2(transform->rect.x, transform->rect.y));

				bool hasNeighbourNode = false;
				int closestNode = 0;
				int size = maxNode - 1; //-1 because the flag is the last node
				for (int i = 0; i < size; i++)
				{
					float distance = (position - _waypoints->getNodes()[i]->getPosition()).length();
					if (distance < FLAG_CONNECTION_RADIUS)
					{
						GraphArc* arcFromFlag = _waypoints->getArc(flagNode, i);
						if (arcFromFlag == nullptr)
						{
							_waypoints->addArc(flagNode, i, distance, false);
						}
						else
						{
							helper::Vector2 flagPos = _waypoints->getNodes()[flagNode]->getPosition();
							helper::Vector2 otherPos = _waypoints->getNodes()[i]->getPosition();
							float distanceBetweenNode = (otherPos - flagPos).length();
							arcFromFlag->setWeight(distanceBetweenNode);
							arcFromFlag->setLine(flagPos, otherPos);

							GraphArc* arcToFlag = _waypoints->getArc(i, flagNode);
							arcToFlag->setWeight(distanceBetweenNode);
							arcToFlag->setLine(flagPos, otherPos);
						}
					}
					else
					{
						if (node->arcList().size() > 1)
						{
							GraphArc* arcFromFlag = _waypoints->getArc(flagNode, i);
							if (arcFromFlag != nullptr)
							{
								node->removeArc(_waypoints->getNodes()[i]);
								_waypoints->getNodes()[i]->removeArc(node);
							}
						}
					}
				}

			}
		}
	}
}