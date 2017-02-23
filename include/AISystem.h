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

	

	void seekFlag(AIComponent* ai, FlagComponent* f)
	{
		if (!f->hasFlag)
		{

		}
		else
		{  
			ai->state = AIState::SeekCheckpoint;
			//transition
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
	}

	void seekCheckpoint(AIComponent* ai, FlagComponent* f)
	{
		if (f->hasFlag)
		{
			//keep going to node
		}
		else
		{
			
		}
	}

	void seekPowerup();

};

