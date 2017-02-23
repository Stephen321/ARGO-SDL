#include "AISystem.h"
#include "FlagComponent.h"


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

				float updateRate = ai->pathFinderUpdateRate;
				float updateTimer = ai->pathfinderUpdateTimer;
				updateTimer += dt;
				if (updateTimer > updateRate)
				{
					updateTimer -= updateRate;
					updateAStar(ai, transform);
				}
				ai->pathfinderUpdateTimer = updateTimer;

				float distance = helper::Vector2(transform->rect.x, transform->rect.y).distance(ai->nextNode->getPosition());
				if (distance < AI_DETECTION_RADIUS && ai->path.size() != 1)
				{
					ai->path.erase(ai->path.begin());
					ai->nextNode = ai->path[0];
				}

				switch (ai->state)
				{
				case AIState::SeekFlag:
					if (ai->nextNode == _flagNode) // go to flag
					{
						if (distance > AI_DETECTION_RADIUS + 50)
						{
							ai->inFlagRange = true;
							//ai->pathfinderUpdateTimer = ai->pathFinderUpdateRate;
						}
					}
					break;
				case AIState::SeekCheckpoint:
					FlagComponent* flagComponent = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					if (flagComponent->hasFlag)
					{
						seekCheckpoint(ai, flagComponent);
					}
					break;
				case AIState::SeekPowerUp:
					break;
				default:
					break;
				}


				if (ai->nextNode != nullptr && !ai->path.empty())
				{
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					//go to path
					
				
					//go to checkpoint
					

					helper::Vector2 AB = ai->nextNode->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
					helper::Vector2 dir = AB.normalize();

					helper::Vector2 force;
					if (ai->path.size() > 1)
					{
						helper::Vector2 AB = ai->path[1]->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
						helper::Vector2 dir = AB.normalize();

						force.x += 0.025f *  dir.x;
						force.y += 0.025f *  dir.y;
					}
					else
					{
						force.x += 0.025f * dir.x;
						force.y += 0.025f * dir.y;
					}

					

					force.x += 0.05f * dir.x;
					force.y += 0.05f * dir.y;

					physics->xAcceleration = force.x;
					physics->yAcceleration = force.y;


					physics->xVelocity += physics->xAcceleration;
					physics->yVelocity += physics->yAcceleration;

				}
			}
		}
	}
}

void AISystem::updateAStar(AIComponent* ai, TransformComponent* t)
{
	if (ai->nextNode == nullptr || (ai->nextNode == _flagNode && ai->inFlagRange))
	{
		ai->inFlagRange = false;
		ai->nextNode = findClosestNode(t);
		
	}
	if (ai->nextNode != _flagNode)
	{
		vector<GraphNode*> path;
		_waypoints->reset();
		//AStar
		_waypoints->setHeuristics(ai->nextNode);
		_waypoints->aStar(ai->nextNode, _flagNode, path);

		//set colour
		_flagNode->setColour(SDL_Color{ 255,0,0,255 });
		ai->nextNode->setColour(SDL_Color{ 0,255,0,255 });

		if (!path.empty())
		{//early check if node is in radius
			int size = path.size() / 2;
			for (int i = 0; i <size; i++)
			{
				float distance = helper::Vector2(t->rect.x, t->rect.y).distance(ai->nextNode->getPosition());
				if (distance < AI_DETECTION_RADIUS)
				{
					path.erase(path.begin());
					ai->nextNode = path[0];
				}
			}
		}
		ai->path = path;
	}
}

GraphNode* AISystem::findClosestNode(TransformComponent* t)
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
	return nodes[index];
}

void AISystem::seekCheckpoint(AIComponent* ai, FlagComponent* f)
{
	if (f->hasFlag)
	{
		int size = _checkpointNode.size();
		for (int i = 0; i < size; i++)
		{
			if (f->currentCheckpointID == _checkpointNode[i]->data().second)
			{
				ai->nextNode = _checkpointNode[i];
				break;
			}
		}
		ai->pathfinderUpdateTimer = ai->pathFinderUpdateRate;
	}
	else
	{

	}
	
}


void seekPowerup();