#include "WaypointSystem.h"
#include "AIComponent.h"
#include "TransformComponent.h"
#include "PowerUpComponent.h"
#include "PhysicsComponent.h"
#include "DestructionComponent.h"

WaypointSystem::WaypointSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests, std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& interactionSystemEvents, float updateRate)
	: System(updateRate)
	, _creationRequests(creationRequests)
	, _interactionSystemEvents(interactionSystemEvents)
	, POWER_UP_DESTRUCTION(InteractionSystemEvent::PowerUpDestoyed)
	, _powerUpCount(0)
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

		CreatePowerUp(dt);
		ListenForEvents();

		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
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
					if (distance < 300.0f)
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

void WaypointSystem::ListenForEvents()
{
	PowerUpDestructionEvent();
}

void WaypointSystem::PowerUpDestructionEvent()
{
	if (!_interactionSystemEvents[POWER_UP_DESTRUCTION].empty())
	{
		std::vector<GraphNode*>& nodes = _waypoints->getNodes();

		for (int i = 0; i < _interactionSystemEvents[POWER_UP_DESTRUCTION].size(); i++)
		{
			PowerUpComponent* powerUp = static_cast<PowerUpComponent*>(_interactionSystemEvents[POWER_UP_DESTRUCTION].at(i).first->GetComponent(Component::Type::PowerUp));

			nodes[powerUp->waypointIndex]->setData("");
			static_cast<DestructionComponent*>(_interactionSystemEvents[POWER_UP_DESTRUCTION].at(i).first->GetComponent(Component::Type::Destroy))->destroy = true;

			_interactionSystemEvents[POWER_UP_DESTRUCTION].erase(_interactionSystemEvents[POWER_UP_DESTRUCTION].begin() + i);
			_powerUpCount--;
			i--;
		}
	}
}

void WaypointSystem::CreatePowerUp(float dt)
{
	if (_powerUpTimer < 0)
	{
		std::vector<GraphNode*>& nodes = _waypoints->getNodes();
		
		int maxPowerUps = nodes.size() * 0.5f;
		int powerUpsToSpawn = rand() % 3 + 1;
		int guard = 0;

		while (powerUpsToSpawn != 0 && _powerUpCount < maxPowerUps)
		{
			int index = rand() % nodes.size();

			if (nodes[index]->data().empty())
			{
				std::vector<float> data = std::vector<float>();

				data.push_back(rand() % ((int)PowerUpComponent::Type::Count)); //id
				data.push_back(nodes[index]->getPosition().x); //xPosition
				data.push_back(nodes[index]->getPosition().y); //yPosition
				data.push_back(index); //node index

				_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::PowerUp, data));
				nodes[index]->setData("PowerUp");
				powerUpsToSpawn--;
				_powerUpCount++;
			}

			guard++;

			if (guard > 1000)
			{
				break;
			}
		}

		_powerUpTimer = POWER_UP_SPAWN_RATE;
	}
	else
	{
		_powerUpTimer -= dt;
	}
}