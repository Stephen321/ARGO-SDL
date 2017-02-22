#include "AISystem.h"

AISystem::AISystem(float updateRate)
	: System(updateRate)
{
}

AISystem::~AISystem()
{

}

void AISystem::Initialize(Graph* waypoints)
{
	_waypoints = waypoints;
	vector<GraphNode*> nodes = _waypoints->getNodes();
	int size = nodes.size();
	for (int i = 0; i < size; i++)
	{
		if (nodes[i]->data().first == GraphNode::EntityData::Checkpoint)
			_checkpointNode.push_back(nodes[i]);
	}
	_flagNode = _waypoints->getNodes()[size - 1];
	
}


void AISystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				AIComponent* ai = static_cast<AIComponent*>(e->GetComponent(Component::Type::AI));
				TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

				float updateRate = ai->PathFinderUpdateRate;
				float updateTimer = ai->pathfinderUpdateTimer;
				updateTimer += dt;
				if (updateTimer > updateRate)
				{
					updateTimer -= updateRate;
					updateAStar(ai, transform);
				}
				ai->pathfinderUpdateTimer = updateTimer;

				if (ai->nextNode != nullptr)
				{
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));

					float distanceToFlag = helper::Vector2(transform->rect.x, transform->rect.y).distance(_flagNode->getPosition());
					if (distanceToFlag < AI_DETECTION_RADIUS)
					{
						ai->path.clear();
						ai->startNode = nullptr;
						ai->nextNode = _flagNode;
					}
					else
					{
						float distance = helper::Vector2(transform->rect.x, transform->rect.y).distance(ai->nextNode->getPosition());
						if (distance < AI_DETECTION_RADIUS)
						{

							ai->path.erase(ai->path.begin());
							ai->nextNode = ai->path[0];

						}
					}



					helper::Vector2 velocity = ai->nextNode->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
					helper::Vector2 dir = velocity.normalize();

					physics->xAcceleration = 0.05f * dir.x;
					physics->yAcceleration = 0.05f * dir.y;


					physics->xVelocity += physics->xAcceleration;
					physics->yVelocity += physics->yAcceleration;

				}



				
				
			}
		}
	}
}

void AISystem::updateAStar(AIComponent* ai, TransformComponent* t)
{
	vector<GraphNode*> path;
	if (ai->startNode == nullptr)
	{
		vector<GraphNode*> nodes = _waypoints->getNodes();
		int size = nodes.size();
		int index = 0;
		float closestDistance = 99999999.f;
		for (int i = 0; i < size; i++)
		{
			float distance = (nodes[i]->getPosition() - helper::Vector2(t->rect.x, t->rect.y)).length();
			if (distance < closestDistance)
			{
				closestDistance = distance;
				index = i;
			}
		}
		ai->startNode = nodes[index];
	}

	_waypoints->reset();
	//AStar
	_waypoints->setHeuristics(ai->startNode);
	_waypoints->aStar(ai->startNode, _flagNode, path);

	//set colour
	_flagNode->setColour(SDL_Color{ 255,0,0,255 });
	ai->startNode->setColour(SDL_Color{ 0,255,0,255 });

	ai->path = path;
	ai->nextNode = ai->path[0];

}