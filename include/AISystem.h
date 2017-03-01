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

	void Seek(AIComponent* ai, FlagComponent* f, TransformComponent* t);

	void Entry(AIComponent* ai, TransformComponent* t);

	void SeekCheckpoint(AIComponent* ai, FlagComponent* f, TransformComponent* t);
	

	void AStarUpdate(AIComponent* ai, TransformComponent* t, float dt);
	void AStar(AIComponent* ai);
	GraphNode* DetermineClosestNode(TransformComponent* t);

	void tranverseNode(AIComponent* ai, TransformComponent* t);
	void Prediction(Entity* currentEntity, AIComponent* ai, TransformComponent* t);

	void AISystem::Avoidance(Entity* self, helper::Vector2& velocity, const helper::Vector2& AIPosition, float force);
	helper::Vector2 CalculateAvoidanceForce(const helper::Vector2& AIPosition, const helper::Vector2& otherPosition, float force);
};

