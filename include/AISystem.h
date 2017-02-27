#pragma once

#include "System.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "Graph.h"

#include "AIComponent.h"
#include "TransformComponent.h" 
#include "FlagComponent.h"

using namespace helper;
class AISystem : public System
{
public:
	AISystem(float updateRate = 0.f);
	~AISystem();

	void Initialize(Graph* waypoints, Entity* player);
	void Process(float dt = 0.f) override;

private:
	Graph* _waypoints;
	GraphNode* _flagNode;
	vector<GraphNode*> _checkpointNode;
	vector<Entity*> _players;

	void Seek(float dt, Entity* e, AIComponent* ai, FlagComponent* f, TransformComponent* t)
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
				float updateRate = ai->pathFinderUpdateRate;
				float updateTimer = ai->pathfinderUpdateTimer;
				//check for closest node
				if (ai->flagDetectionRange)
				{
					ai->flagDetectionRange = false; //not in anymore, call and determin closest node
					ai->nextNode = DetermineClosestNode(t);
					updateTimer = 0;
				}
				//update Astar
		
				updateTimer += dt;
				if (updateTimer > updateRate)
				{
					updateTimer -= updateRate;
					AStar(ai);
				}
				ai->pathfinderUpdateTimer = updateTimer;
				Prediction();
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
		}
	}

	void Entry(AIComponent* ai, TransformComponent* t);
	void SeekFlag(Entity* e, AIComponent* ai, FlagComponent* f, TransformComponent* t);

	void SeekPrediction()
	{

	}


	void SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t);
	void SeekPowerup()
	{

	}

	void UsePowerup()
	{

	}

	void AStar(AIComponent* ai);
	GraphNode* DetermineClosestNode(TransformComponent* t);

	void Prediction(Entity* currentEntity, AIComponent* ai, TransformComponent* t)
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

		if (entityWithFlag != nullptr)
		{
			//loop through AI and find AI with flag
			for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
			{
				for (Entity* AIEntities : (*it).second)
				{
					if (currentEntity != AIEntities)
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
		
		
			FlagComponent* f = static_cast<FlagComponent*>(entityWithFlag->GetComponent(Component::Type::Flag));
			int size = _checkpointNode.size();
			int target = f->currentCheckpointID + 1;
			if (target > 4)
			{
				target = 0;
			}
			for (int i = 0; i < size; i++)
			{
				if (target == _checkpointNode[i]->data().second)
				{
					
					float distanceAIToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));

					TransformComponent* transform = static_cast<TransformComponent*>(entityWithFlag->GetComponent(Component::Type::Transform));
					float flagOwnerDistanceToCheckpoint = _checkpointNode[i]->getPosition().distance(helper::Vector2(transform->rect.x, transform->rect.y));

				
					if (distanceAIToCheckpoint > flagOwnerDistanceToCheckpoint + AI_TO_CHASE_RANGE_OFFSET)
					{
						ai->destNode = _checkpointNode[i];
						ai->nextNode = DetermineClosestNode(t);
						ai->state = AIState::Camp;
					}
					break;
				}
			}
		}

	
	}

	void Shoot()
	{

	}
};

