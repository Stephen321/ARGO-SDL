#include "AISystem.h"

AISystem::AISystem(float updateRate)
	: System(updateRate)
{
}

AISystem::~AISystem()
{

}

void AISystem::Initialize(Graph* waypoints, Entity* player)
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
	_players.push_back(player);

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
					Seek(dt, e, ai, flag, transform);
					if (ai->path.size() > 2)
					{
						float distance = ai->nextNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
						int radii = AI_NODE_COLLISION_RADIUS + NODE_RADIUS;
						//if ((AB.x*AB.x + AB.y*AB.y) < radii*radii)
						if (distance < AI_NODE_COLLISION_RADIUS)
						{
							ai->path.erase(ai->path.begin());
							ai->nextNode = ai->path.front();
						}

					}
					break;
				}
				case AIState::Camp:
				{
					//FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					//SeekCheckpoint(ai, flag, transform);
					break;
				}
				case AIState::SeekCheckpoint:
				{
					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					SeekCheckpoint(ai, flag, transform);
					if (ai->path.size() > 1)
					{
						float distance = ai->nextNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
						int radii = AI_NODE_COLLISION_RADIUS + NODE_RADIUS;
						//if ((AB.x*AB.x + AB.y*AB.y) < radii*radii)
						if (distance < AI_NODE_COLLISION_RADIUS)
						{
							ai->path.erase(ai->path.begin());
							ai->nextNode = ai->path[0];
						}
					}
					break;
				}
				
				case AIState::SeekPowerUp:
					break;
				default:
					break;
				}

				

				

					


				
				
	
				


				if(!ai->path.empty())
				{
					/*
					//eltaX = tx - cx;
					//deltaY = ty - cy;
					distance = sqrt(deltaX ^ 2 + deltaY ^ 2);
					dvx = deltaX * maxSpeed / distance; //Normalizing and multiplying by max speed
					dvy = deltaY * maxSpeed / distance;
					deltaX = dvx - vx;
					deltaY = dvy - vy;
					diffSize = sqrt(deltaX ^ 2 + deltaY ^ 2);
					ax = maxAcc * deltaX / diffSize;
					ay = maxAcc * deltaY / diffSize;

					vx += ax * dt // dt is the time that passed since the last frame
						vy += ay * dt // 
						*/
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					helper::Vector2 AB = ai->nextNode->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
					//go to path


					//go to checkpoint
					
	
				
					

					
					
					/*
					
					if (ai->path.size() > 1)
					{

						helper::Vector2 midpoint = (ai->nextNode->getPosition() + ai->path[1]->getPosition()) * 0.5f;
						AB = midpoint - helper::Vector2(transform->rect.x, transform->rect.y);
						
						

						//helper::Vector2 dot = x1*x2 + y1*y2;
						//helper::Vector2 det = x1*y2 - y1*x2;
						//angle = atan2(det, dot)
					}*/

					

				
					helper::Vector2 direction = AB.normalize();
					physics->xDir = direction.x;
					physics->yDir = direction.y;

					float force = 4.0f;
					physics->xAcceleration = force;
					physics->yAcceleration = force;


					//physics->xVelocity += force.x* dt;
					//physics->yVelocity += force.y * dt;


					//if(direction.x )
					/*
					if (physics->xVelocity > physics->maxVelocity)
					{
						physics->xVelocity = physics->maxVelocity;
					}
					if (physics->yVelocity > physics->maxVelocity)
					{
						physics->yVelocity = physics->maxVelocity;
					}*/


					
	
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
		if (distance < closestNodeDistance && distance < AI_NODE_COLLISION_RADIUS)
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
	//_waypoints->aStar(ai->nextNode, ai->destNode, path);
	_waypoints->aStar(ai->nextNode , ai->destNode, path);
	//set colouraaa
	ai->destNode->setColour(SDL_Color{ 255,0,0,255 });
	ai->nextNode->setColour(SDL_Color{ 0,255,0,255 });

	if (!path.empty())
	{
		ai->path = path;
		
	}
}

void AISystem::Entry(AIComponent* ai, TransformComponent* t)
{
	//transition to SeekFlag
	ai->state = AIState::SeekFlag;
	ai->nextNode = DetermineClosestNode(t); //AI doesn't know starting node, so we search for the closest one. this is a manual update.
	ai->destNode = _flagNode;
	ai->pathfinderUpdateTimer = 0;
	AStar(ai);
}

void AISystem::SeekFlag(Entity* e,AIComponent* ai, FlagComponent* f, TransformComponent* t)
{
	/*

	if (!f->hasFlag)
	{
		//don't have flag, check for distance to flag and then set go to node!
		float distanceToFlag = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
		if (distanceToFlag > AI_FLAG_DETECTION_RANGE + 200)
		{

			FlagPrediction(e, ai, t);
		}
		else if (distanceToFlag < AI_FLAG_DETECTION_RANGE)
		{
			ai->nextNode = ai->destNode;
			ai->updateAStarFlag = false; // AI is in range, stop astar call
			ai->closestNodeFlag = false; // stop checking for closest node.
			ai->flagDetectionRange = true;
		}
		
		else if(distanceToFlag > AI_FLAG_DETECTION_RANGE + 50)
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
	}*/
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
				if (f->currentCheckpointID + 1== _checkpointNode[i]->data().second)
				{
					ai->destNode = _checkpointNode[i];
					break;
				}
			}
			ai->nextNode = DetermineClosestNode(t);
			AStar(ai);   //manual call on AStar.
		}
	}
	else
	{
		//lose flag, transition to seek flag 
		ai->state = AIState::SeekFlag;
		ai->destNode = _flagNode;
		ai->nextNode = DetermineClosestNode(t);	
		//ai->pathfinderUpdateTimer = 0;
		//AStar(ai);
	}
}