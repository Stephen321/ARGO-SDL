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

				switch (ai->state)
				{
				case AIState::Entry:             
					Entry(ai, transform);
					break;
				case AIState::SeekFlag:
				{
					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					SeekFlag(ai, flag, transform);
					break;
				}
				case AIState::SeekCheckpoint:
				{
					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					SeekCheckpoint(ai, flag, transform);
				}
					break;
				case AIState::SeekPowerUp:
					break;
				default:
					break;
				}

				if (ai->updateAStarFlag)
				{
					float updateRate = ai->pathFinderUpdateRate;
					float updateTimer = ai->pathfinderUpdateTimer;

					updateTimer += dt;
					if (updateTimer > updateRate)
					{
						updateTimer -= updateRate;
						AStar(ai);
					}
					ai->pathfinderUpdateTimer = updateTimer;
				}

				
				if (ai->path.size() > 2)
				{
					float distance = ai->nextNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
					if (distance < AI_NODE_COLLISION_RADIUS)
					{

						ai->path.erase(ai->path.begin());

						ai->nextNode = ai->path[0];

					}
				}
				
	
				


				if(!ai->path.empty())
				{

					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					//go to path


					//go to checkpoint


					helper::Vector2 AB = ai->nextNode->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
					helper::Vector2 firstNodeDir = AB.normalize();

					helper::Vector2 force;
					
					if (ai->path.size() > 1)
					{
						helper::Vector2 AB = ai->path[1]->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
						helper::Vector2 SecondNodeDir = AB.normalize();

						force.x += 0.015f *  SecondNodeDir.x;
						force.y += 0.015f *  SecondNodeDir.y;
						force.x += 0.05f * firstNodeDir.x;
						force.y += 0.05f * firstNodeDir.y;
					}
					else
					{
						force.x += 0.065f * firstNodeDir.x;
						force.y += 0.065f * firstNodeDir.y;
					}
		
					
					physics->xAcceleration = force.x;
					physics->yAcceleration = force.y;


					physics->xVelocity += force.x ;
					physics->yVelocity += force.y ;
	
				}
			}
		}
	}
}




GraphNode* AISystem::DetermineClosestNode(TransformComponent* t)
{
	//loop through the nodes and determine the closest node.
	vector<GraphNode*> nodes = _waypoints->getNodes();

	int closestNodeIndex = 0;
	float closestNodeDistance = 99999999.f;

	int size = nodes.size();
	for (int i = 0; i < size; i++)
	{
		float distance = (nodes[i]->getPosition() - helper::Vector2(t->rect.x, t->rect.y)).length();
		if (distance < closestNodeDistance)
		{
			closestNodeDistance = distance;
			closestNodeIndex = i;
		}
	}
	return nodes[closestNodeIndex];
}

void AISystem::AStar(AIComponent* ai)
{
	vector<GraphNode*> path;

	_waypoints->reset();
	_waypoints->setHeuristics(ai->nextNode);

	//call AStar
	_waypoints->aStar(ai->nextNode, ai->destNode, path);

	//set colour
	ai->destNode->setColour(SDL_Color{ 255,0,0,255 });
	ai->nextNode->setColour(SDL_Color{ 0,255,0,255 });

	if (!path.empty())
		ai->path = path;
}

void AISystem::Entry(AIComponent* ai, TransformComponent* t)
{
	//transition to SeekFlag
	ai->state = AIState::SeekFlag;
	ai->nextNode = DetermineClosestNode(t); //AI doesn't know starting node, so we search for the closest one. this is a manual update.
	ai->destNode = _flagNode;
	ai->pathfinderUpdateTimer = 0;
	ai->closestNodeFlag = true; // flag that determine closest
	ai->updateAStarFlag = true;
	AStar(ai);
}

void AISystem::SeekFlag(AIComponent* ai, FlagComponent* f, TransformComponent* t)
{
	if (!f->hasFlag)
	{
		//don't have flag, check for distance to flag and then set go to node!
		float distanceToFlag = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
		if (distanceToFlag < AI_FLAG_DETECTION_RANGE)
		{
			ai->nextNode = ai->destNode;
			ai->updateAStarFlag = false; // AI is in range, stop astar call
			ai->closestNodeFlag = false; // stop checking for closest node.
			ai->flagDetectionRange = true;
		}
		else if (distanceToFlag > AI_FLAG_DETECTION_RANGE)
		{
			ai->updateAStarFlag = true;
			ai->closestNodeFlag = true; // turn on checking for closest node
			if (ai->flagDetectionRange && ai->closestNodeFlag)
			{
				ai->flagDetectionRange = false; //not in anymore, call and determin closest node
				ai->nextNode = DetermineClosestNode(t);
			}
		}
	}
	else
	{
		//has flag, transition to seekCheckpoint
		ai->state = AIState::SeekCheckpoint;

		int size = _checkpointNode.size();
		for (int i = 0; i < size; i++)
		{
			if (f->currentCheckpointID == _checkpointNode[i]->data().second)
			{
				ai->destNode = _checkpointNode[i];
				break;
			}
		}
		ai->pathfinderUpdateTimer = ai->pathFinderUpdateRate; //set timer to update AStar
		AStar(ai);   //manual call on AStar.
		ai->updateAStarFlag = false; //turn off AStar on nodes.
	}
}

void AISystem::SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t)
{
	if (f->hasFlag)
	{
		//keep going to checkpoint and check collision with it.
		float distanceToCheckpoint = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
		if (distanceToCheckpoint < CHECKPOINT_COLLISION_RADIUS)
		{
			//f->currentCheckpointID++; 
			//reach checkpoint, increase and search for next waypoint node.
			int size = _checkpointNode.size();
			for (int i = 0; i < size; i++)
			{
				if (f->currentCheckpointID == _checkpointNode[i]->data().second)
				{
					ai->destNode = _checkpointNode[i];
					break;
				}
			}
			ai->pathfinderUpdateTimer = ai->pathFinderUpdateRate; //set timer to update AStar
			AStar(ai);   //manual call on AStar.
		}
	}
	else
	{
		//lose flag, transition to seek flag 
		ai->state = AIState::SeekFlag;
		ai->destNode = _flagNode;
		ai->pathfinderUpdateTimer = ai->pathFinderUpdateRate; //set timer to update AStar
	}
}