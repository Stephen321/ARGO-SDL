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
				PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));

				helper::Vector2 velocity = helper::Vector2(0, 0);

				helper::Vector2 AIPosition = helper::Vector2(transform->rect.x, transform->rect.y);

				switch (ai->state)
				{
				case AIState::Entry:             
					Entry(ai, transform);
					break;
				case AIState::SeekFlag:
				{
					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					Seek(ai, flag, transform);
					if (!ai->flagDetectionRange)
					{
						//loop through all players and calculate distance and then add avoidance force
						for (int i = 0; i < _players.size(); i++)
						{
							FlagComponent* playerFlag = static_cast<FlagComponent*>(_players[i]->GetComponent(Component::Type::Flag));
							if (!playerFlag->hasFlag)
							{
								TransformComponent* playerTransfrom = static_cast<TransformComponent*>(_players[i]->GetComponent(Component::Type::Transform));
								velocity += Avoidance(AIPosition, helper::Vector2(playerTransfrom->rect.x, playerTransfrom->rect.y), 2.0f);
							}
						}
						//loop through all AI and calculate distance and then add avoidance force
						for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
						{
							for (Entity* otherAI : (*it).second)
							{
								if (e != otherAI) //check if not self.
								{
									FlagComponent* otherAIFlag = static_cast<FlagComponent*>(otherAI->GetComponent(Component::Type::Flag));
									if (!otherAIFlag->hasFlag)
									{
										TransformComponent* otherAITransfrom = static_cast<TransformComponent*>(otherAI->GetComponent(Component::Type::Transform));
										velocity += Avoidance(AIPosition, helper::Vector2(otherAITransfrom->rect.x, otherAITransfrom->rect.y), 2.0f);
									}
								}
							}
						}

						//update Astar
						float updateRate = ai->pathFinderUpdateRate;
						float updateTimer = ai->pathfinderUpdateTimer;
						updateTimer += dt;
						if (updateTimer > updateRate)
						{
							std::cout << "calling ASTAR!!!";
							updateTimer -= updateRate;
							AStar(ai);
							//ai->nextNode = ai->path.front();
						}
						ai->pathfinderUpdateTimer = updateTimer;

						Prediction(e, ai, transform);
					}
					
					//Move along the Path
					if (ai->path.size() > 2)
					{
						float distance = ai->nextNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
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
					if (ai->path.size() > 1)
					{
						float distance = ai->nextNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
						if (distance < AI_NODE_COLLISION_RADIUS)
						{
							ai->path.erase(ai->path.begin());
							ai->nextNode = ai->path.front();
						}
					}
					else
					{
						float distance = _flagNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
						if (distance < AI_TO_CHASE_RANGE_OFFSET + AI_TO_CHASE_RANGE_OFFSET + AI_TO_CHASE_RANGE_OFFSET)
						{
							ai->state = AIState::SeekFlag;
							ai->destNode = _flagNode;
						}
					}

					break;
				}
				case AIState::SeekCheckpoint:
				{
					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					SeekCheckpoint(ai, flag, transform);
				

					//loop through all players and calculate distance and then add avoidance force
					for (int i = 0; i < _players.size(); i++)
					{
						TransformComponent* playerTransfrom = static_cast<TransformComponent*>(_players[i]->GetComponent(Component::Type::Transform));
						velocity += Avoidance(AIPosition, helper::Vector2(playerTransfrom->rect.x, playerTransfrom->rect.y), 6.f);
					}
					//loop through all AI and calculate distance and then add avoidance force
					for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
					{
						for (Entity* otherAI : (*it).second)
						{
							if (e != otherAI) //check if not self.
							{
								TransformComponent* otherAITransfrom = static_cast<TransformComponent*>(otherAI->GetComponent(Component::Type::Transform));
								velocity += Avoidance(AIPosition, helper::Vector2(otherAITransfrom->rect.x, otherAITransfrom->rect.y), 6.f);
							}
						}
					}

					//update Astar
					float updateRate = ai->pathFinderUpdateRate;
					float updateTimer = ai->pathfinderUpdateTimer;		
					updateTimer += dt;
					if (updateTimer > updateRate)
					{
						std::cout << "calling ASTAR!!!";
						updateTimer -= updateRate;
						ai->nextNode = DetermineClosestNode(transform);
						AStar(ai);
						ai->nextNode = ai->path.front();
					}
					ai->pathfinderUpdateTimer = updateTimer;

					//Move along the Path
					if (ai->path.size() > 1)
					{
						float distance = ai->nextNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
						if (distance < AI_NODE_COLLISION_RADIUS)
						{
							ai->path.erase(ai->path.begin());
							ai->nextNode = ai->path.front();
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
					helper::Vector2 nextNodeVector2 = ai->nextNode->getPosition() - AIPosition;
					helper::Vector2 waypointDirection = nextNodeVector2.normalize();


					float force = 4.0f;

					velocity += waypointDirection * 4.0f;			
				}

				float xDrag = -physics->xVelocity * DRAG;
				float yDrag = -physics->yVelocity * DRAG;
				physics->xVelocity += (velocity.x + xDrag) * dt;
				physics->yVelocity += (velocity.y + yDrag) * dt;
			}
		}
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

void AISystem::Seek(AIComponent* ai, FlagComponent* f, TransformComponent* t)
{
	if (!f->hasFlag)
	{
		//don't have flag, check for distance to flag and then set go to node!
		float distanceToDest = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
		if (distanceToDest < AI_FLAG_DETECTION_RADIUS)
		{
			ai->nextNode = ai->destNode;
			ai->flagDetectionRange = true;
		}
		else
		{
			if (ai->flagDetectionRange)
			{
				ai->flagDetectionRange = false; //not in anymore, call and determin closest node
				ai->nextNode = DetermineClosestNode(t);
				ai->pathfinderUpdateTimer = 0;
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
			if (f->currentCheckpointID + 1 == _checkpointNode[i]->data().second)
			{
				ai->destNode = _checkpointNode[i];
				break;
			}
		}
		ai->pathfinderUpdateTimer = 0;
		AStar(ai);   //manual call on AStar.

	}
}

void AISystem::Prediction(Entity* self, AIComponent* ai, TransformComponent* t)
{
	Entity* entityWithFlag = nullptr;

	//loop through players and find player with flag
	int size = _players.size();
	for (int i = 0; i < size; i++)
	{
		FlagComponent* f = static_cast<FlagComponent*>(_players[i]->GetComponent(Component::Type::Flag));
		if (f->hasFlag)
		{
			entityWithFlag = _players[i];
			break;
		}
	}

	
		//loop through AI and find AI with flag
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* AIEntities : (*it).second)
			{
				if (self != AIEntities)
				{
					FlagComponent* f = static_cast<FlagComponent*>(AIEntities->GetComponent(Component::Type::Flag));
					if (f->hasFlag)
					{
						entityWithFlag = AIEntities;
						break;
					}
				}
			}
		}
	
		if (entityWithFlag != nullptr)
		{
			FlagComponent* f = static_cast<FlagComponent*>(entityWithFlag->GetComponent(Component::Type::Flag));
			int size = _checkpointNode.size();
			int target = f->currentCheckpointID + 1;
			for (int i = 0; i < size; i++)
			{
				if (target == _checkpointNode[i]->data().second)
				{

					float selfDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));

					TransformComponent* transform = static_cast<TransformComponent*>(entityWithFlag->GetComponent(Component::Type::Transform));
					float otherDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));


					if (selfDistanceToCheckpoint > otherDistanceToCheckpoint + AI_TO_CHASE_RANGE_OFFSET )
					{
						target++;
						if (target > 4)
							target = 1;
						for (int j = 0; j < size; j++)
						{
							if (target == _checkpointNode[j]->data().second)
							{
								ai->destNode = _checkpointNode[j];
								break;
							}
						}

						ai->nextNode = DetermineClosestNode(t);
						AStar(ai);
						ai->state = AIState::Camp;
					}
					break;
				}
			}
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
		ai->pathfinderUpdateTimer = 0;
		//AStar(ai);
	}
}

helper::Vector2& AISystem::Avoidance(const helper::Vector2& AIPosition, const helper::Vector2& otherPosition, float force)
{
	helper::Vector2 velocity = helper::Vector2(0, 0);
	helper::Vector2 AB = AIPosition - otherPosition;
	if (AB.length() < 250) // distance to AI
	{
		helper::Vector2 direction = AB.normalize();
		velocity += direction * force;
	}
	return velocity;
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
		if ( distance < AI_NODE_COLLISION_RADIUS && distance < closestNodeDistance)
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
	_waypoints->aStar(ai->nextNode, ai->destNode, path);
	//set colouraaa
	ai->destNode->setColour(SDL_Color{ 255,0,0,255 });
	ai->nextNode->setColour(SDL_Color{ 0,255,0,255 });

	if (!path.empty())
	{
		ai->path = path;

	}
}