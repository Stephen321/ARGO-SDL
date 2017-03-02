#pragma once

#include "System.h"
#include "PhysicsSystem.h"

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

	void SeekFlag(AIComponent* ai, FlagComponent* f, TransformComponent* t, float distanceToDest);

	void Entry(AIComponent* ai, TransformComponent* t);

	void SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t);
	
	pair<float, int> DeterminePowerUp(helper::Vector2& velocity, AIComponent* ai, TransformComponent* t)
	{
		std::vector<GraphNode*> nodes = _waypoints->getNodes();
		int size = _waypoints->getNodesSize();
		int closestIndex;
		float closestPowerup = 999999999.0f;
		for (int i = 0;i < size;i++)
		{
			if (nodes[i]->data().first == GraphNode::EntityData::PowerUp)
			{
				
				float distanceToPowerUp = nodes[i]->getPosition().distance(helper::Vector2(t->rect.x, t->rect.x));
				if (distanceToPowerUp < closestPowerup)
				{
					closestPowerup = distanceToPowerUp;
					closestIndex = i;
				}
			}
		}

		return make_pair(closestPowerup, closestIndex);
		
	}

	void AStarUpdate(AIComponent* ai, TransformComponent* t, float dt);
	void AStar(AIComponent* ai);
	GraphNode* DetermineClosestNode(TransformComponent* t);

	void TranverseNode(AIComponent* ai, TransformComponent* t);
	void Prediction(Entity* entityWithFlag, AIComponent* ai, TransformComponent* t);

	void Avoidance(Entity* self, helper::Vector2& velocity, const helper::Vector2& AIPosition, float force);
	helper::Vector2 CalculateAvoidanceForce(const helper::Vector2& AIPosition, const helper::Vector2& otherPosition, float force);

	Entity* FindEntityWithFlag(Entity* self);
};

