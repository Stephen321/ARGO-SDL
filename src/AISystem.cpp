#include "AISystem.h"
#include "StatusEffectComponent.h"
#include "PhysicsComponent.h"
#include "GunComponent.h"
#include "WeaponComponent.h"

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
				WeaponComponent* weapon = static_cast<WeaponComponent*>(e->GetComponent(Component::Type::Weapon));

				helper::Vector2 velocity = helper::Vector2(0, 0);

				helper::Vector2 AIPosition = helper::Vector2(transform->rect.x, transform->rect.y);

				switch (ai->state)
				{
				case AIState::Entry:
					Entry(ai, transform);
					break;
				case AIState::SeekFlag:
				{
					float force = 1.0f;

					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));

					//don't have flag, check for distance to flag and then set go to node!
					float distanceToDest = ai->destNode->getPosition().distance(AIPosition);

					SeekFlag(ai, flag, transform, distanceToDest);
					if (!ai->flagDetectionRange)
					{	
						Entity* entityWithFlag = FindEntityWithFlag(e);
						if (entityWithFlag != nullptr)
						{
							force = 0.0;
							//Determine if there is powerup to take		
								
							if (!weapon->hasWeapon)
							{
								
								std::pair<float, int> powerUpData = DeterminePowerUp(velocity, ai, transform);
								if (powerUpData.first < AI_POWER_UP_RADIUS)
								{

									ai->nextNode = DetermineClosestNode(transform);
									ai->destNode = _waypoints->getNodes()[powerUpData.second];
									AStar(ai);
									ai->state = AIState::SeekPowerUp;
									break;
								}
							}
							//prediction to checkpoint
							Prediction(entityWithFlag, ai, transform);
						}
					}
					
					//update Astar
					AStarUpdate(ai, transform, dt);
					

					Avoidance(e, velocity, AIPosition, force);
					
					Entity* entityWithFlag = FindEntityWithFlag(e);
					if (entityWithFlag == nullptr)
					{
						//loop through all players and calculate distance and then add avoidance force
						for (int i = 0; i < _players.size(); i++)
						{
							TransformComponent* playerTransfrom = static_cast<TransformComponent*>(_players[i]->GetComponent(Component::Type::Transform));
							velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(playerTransfrom->rect.x, playerTransfrom->rect.y), force);
						}
						//loop through all AI and calculate distance and then add avoidance force
						for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
						{
							for (Entity* otherAI : (*it).second)
							{
								if (e != otherAI && otherAI != entityWithFlag) //check if not self.
								{
									TransformComponent* otherAITransfrom = static_cast<TransformComponent*>(otherAI->GetComponent(Component::Type::Transform));
									velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(otherAITransfrom->rect.x, otherAITransfrom->rect.y), force);
								}
							}
						}
					}

					
					//Move along the Path
					if (ai->path.size() > 2)
					{
						TranverseNode(ai, transform);
					}
					break;
				}
				case AIState::Camp:
				{
					float distance = _flagNode->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));
					if (ai->path.size() > 1)
					{
						TranverseNode(ai, transform);
					}
					else
					{
						
						if (distance < AI_CAMP_FLAG_DETECTION_RANGE)
						{
							ai->state = AIState::SeekFlag;
							ai->destNode = _flagNode;
						}
						
						//prediction
						Entity* entityWithFlag = FindEntityWithFlag(e);
						if (entityWithFlag != nullptr)
						{
							Prediction(entityWithFlag, ai, transform);
						}
						else
						{
							ai->state = AIState::SeekFlag;
							ai->destNode = _flagNode;
						}
					}

					if (distance < AI_CHASE_RADIUS)
					{
						ai->state = AIState::SeekFlag;
						ai->destNode = _flagNode;
					}

					Avoidance(e, velocity, AIPosition, AI_WAYPOINT_FORCE + 2.0f);

					break;
				}
				case AIState::SeekCheckpoint:
				{
					FlagComponent* flag = static_cast<FlagComponent*>(e->GetComponent(Component::Type::Flag));
					SeekCheckpoint(ai, flag, transform);
					Avoidance(e, velocity, AIPosition, AI_WAYPOINT_FORCE + 2.0f);
					AStarUpdate(ai, transform, dt);

					//Move along the Path
					if (ai->path.size() > 1)
					{
						TranverseNode(ai, transform);
					}

					break;
				}
				case AIState::SeekPowerUp:
				{

					//update Astar
					Avoidance(e, velocity, AIPosition, 4.0f);
					AStarUpdate(ai, transform, dt);
					if (ai->path.size() > 1)
					{
						TranverseNode(ai, transform);
					}
					if (ai->destNode->data().first == GraphNode::EntityData::Null) //someone took power, return to seek
					{
						
						ai->destNode = _flagNode;
						ai->state = AIState::SeekFlag;

					}
					break;
				}
				default:
					break;
				}


				if (weapon->hasWeapon)
				{
					Entity* entityWithFlag = FindEntityWithFlag(e);
					if(entityWithFlag != nullptr)
					{
						TransformComponent* otherTransform = static_cast<TransformComponent*>(entityWithFlag->GetComponent(Component::Type::Transform));
						if (AIPosition.distance(helper::Vector2(otherTransform->rect.x, otherTransform->rect.y)) < 200)
						{
							//weapon->fired = true;
						}
					}
					
				}

				if(!ai->path.empty())
				{
					helper::Vector2 nextNodeVector2 = ai->nextNode->getPosition() - AIPosition;
					helper::Vector2 waypointDirection = nextNodeVector2.normalize();

					velocity += waypointDirection * AI_WAYPOINT_FORCE;
				}

				float maxVelocity = 8.0f;
				float currentVelocity = sqrt(velocity.x* velocity.x + velocity.y * velocity.y);
				if (currentVelocity > maxVelocity)
				{
					velocity = velocity.normalize() * maxVelocity;
				}

				StatusEffectComponent* statusEffect = static_cast<StatusEffectComponent*>(e->GetComponent(Component::Type::StatusEffect));
				if (!statusEffect->staggered)
				{
					float xDrag = -physics->xVelocity * DRAG;
					float yDrag = -physics->yVelocity * DRAG;
					physics->xVelocity += (velocity.x + xDrag) * dt;
					physics->yVelocity += (velocity.y + yDrag) * dt;
				}		
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

void AISystem::SeekFlag(AIComponent* ai, FlagComponent* f, TransformComponent* t, float distanceToDest)
{
	if (!f->hasFlag)
	{
		
		if (distanceToDest < AI_CHASE_RADIUS)
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

void AISystem::Prediction(Entity* entityWithFlag, AIComponent* ai, TransformComponent* t)
{
	FlagComponent* f = static_cast<FlagComponent*>(entityWithFlag->GetComponent(Component::Type::Flag));
	int size = _checkpointNode.size();
	int target = f->currentCheckpointID + 1;
	for (int i = 0; i < size; i++)
	{
		if (target == _checkpointNode[i]->data().second)
		{

			float selfDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));

			TransformComponent* entityWIthTransform = static_cast<TransformComponent*>(entityWithFlag->GetComponent(Component::Type::Transform));
			float otherDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(entityWIthTransform->rect.x, entityWIthTransform->rect.y));
			float weighDistance = selfDistanceToCheckpoint - otherDistanceToCheckpoint; //difference between entity pos to checkpoint and entity with flag pos to checkpoint
			if (weighDistance > AI_TO_CHASE_RANGE_OFFSET)
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

void AISystem::SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t)
{
	if (f->hasFlag)
	{
		//keep going to checkpoint and check collision with it.
		float distanceToCheckpoint = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
		if (distanceToCheckpoint < CHECKPOINT_COLLISION_RADIUS)
		{
			ai->nextNode = ai->destNode;
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
			//ai->nextNode = DetermineClosestNode(t);
			//AStar(ai);   //manual call on AStar.
			ai->pathfinderUpdateTimer = 0;
		}
	}
	else
	{
		//lose flag, transition to seek flag 
		ai->state = AIState::SeekFlag;
		ai->destNode = _flagNode;
		ai->nextNode = DetermineClosestNode(t);	
		ai->pathfinderUpdateTimer = 0;
	}
}

void AISystem::Avoidance(Entity* self, helper::Vector2& velocity, const helper::Vector2& AIPosition, float force)
{
	//loop through all players and calculate distance and then add avoidance force
	for (int i = 0; i < _players.size(); i++)
	{
		TransformComponent* playerTransfrom = static_cast<TransformComponent*>(_players[i]->GetComponent(Component::Type::Transform));
		velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(playerTransfrom->rect.x, playerTransfrom->rect.y), force);
	}
	//loop through all AI and calculate distance and then add avoidance force
	for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
	{
		for (Entity* otherAI : (*it).second)
		{
			if (self != otherAI) //check if not self.
			{
				TransformComponent* otherAITransfrom = static_cast<TransformComponent*>(otherAI->GetComponent(Component::Type::Transform));
				velocity += CalculateAvoidanceForce(AIPosition, helper::Vector2(otherAITransfrom->rect.x, otherAITransfrom->rect.y), force);
			}
		}
	}
}

helper::Vector2 AISystem::CalculateAvoidanceForce(const helper::Vector2& AIPosition, const helper::Vector2& otherPosition, float force)
{
	helper::Vector2 velocity = helper::Vector2(0, 0);
	helper::Vector2 AB = AIPosition - otherPosition;
	if (AB.length() < AI_AVOIDANCE_RADIUS) // distance to AI
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

void AISystem::TranverseNode(AIComponent* ai, TransformComponent* t)
{
	float distance = ai->nextNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
	if (distance < AI_NODE_COLLISION_RADIUS)
	{
		ai->path.erase(ai->path.begin());
		ai->nextNode = ai->path.front();
	}
}

void AISystem::AStarUpdate(AIComponent* ai, TransformComponent* t ,float dt)
{
	//update Astar
	float updateRate = ai->pathFinderUpdateRate;
	float updateTimer = ai->pathfinderUpdateTimer;
	updateTimer += dt;
	if (updateTimer > updateRate)
	{
		std::cout << "calling ASTAR!!!";
		updateTimer -= updateRate;
		AStar(ai);
	}
	ai->pathfinderUpdateTimer = updateTimer;
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
	{
		ai->path = path;
	}
}

Entity* AISystem::FindEntityWithFlag(Entity* self)
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

	return entityWithFlag;
}