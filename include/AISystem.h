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

	void Initialize(Graph* waypoints);
	void Process(float dt = 0.f) override;

private:
	Graph* _waypoints;
	GraphNode* _flagNode;
	vector<GraphNode*> _checkpointNode;

	void updateAStar(AIComponent* ai, TransformComponent* t);
	GraphNode* findClosestNode(TransformComponent* t);

	

	void Entry(AIComponent* ai, TransformComponent* t)
	{
		//transition to SeekFlag
		ai->state = AIState::SeekFlag;
		ai->nextNode = DetermineClosestNode(t); //AI doesn't know starting node, so we search for the closest one. this is a manual update.
		ai->destNode = _flagNode; 
		ai->pathfinderUpdateTimer = ai->pathFinderUpdateRate; //set timer to update AStar
		ai->closestNodeFlag = true; // flag that determine closest
		ai->updateAStarFlag = true;
		AStar(ai); 
	}

	void SeekFlag(AIComponent* ai, FlagComponent* f, TransformComponent* t)
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
				ai->flagDetectionRange = true; // in range of flag.
			}
			else
			{
				ai->closestNodeFlag = true; // turn on checking for closest node
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

	void SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t)
	{
		if (f->hasFlag)
		{
			//keep going to checkpoint and check collision with it.
			float distanceToCheckpoint = ai->destNode->getPosition().distance(helper::Vector2(t->rect.x, t->rect.y));
			if (distanceToCheckpoint < CHECKPOINT_COLLISION_RADIUS)
			{
				f->currentCheckpointID++; 
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

	void SeekPowerup()
	{

	}

	void UsePowerup()
	{

	}

	void AStar(AIComponent* ai)
	{
		vector<GraphNode*> path;

		_waypoints->reset();
		_waypoints->setHeuristics(ai->nextNode);

		//call AStar
		_waypoints->aStar(ai->nextNode, ai->destNode, path);

		//set colour
		ai->destNode->setColour(SDL_Color{ 255,0,0,255 });
		ai->nextNode->setColour(SDL_Color{ 0,255,0,255 });

		if(!path.empty())
			ai->path = path;
	}

	GraphNode* DetermineClosestNode(TransformComponent* t)
	{
		///////////////////////////put code in process///////////////////////////////
		//if (ai->flagDetectionRange && ai->closestNodeFlag)
		//{
		//	ai->flagDetectionRange = false; //not in anymore, call and determin closest node
		//}
		//////////////////////////////////////////////////////////

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
};

