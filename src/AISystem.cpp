#include "AISystem.h"
#include "AIComponent.h"
#include "TransformComponent.h" 
AISystem::AISystem(float updateRate)
	: System(updateRate)
{
}

AISystem::~AISystem()
{

}

void AISystem::Initialize(Graph* map)
{
	_map = map;
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
				float updateRate = ai->updateRate;
				ai->updateTimer += dt;
				if (ai->updateTimer > updateRate)
				{
				
				
					ai->updateTimer -= updateRate;

					/*
					if (!ai->callAstar)//testing
					{*/

					_map->reset();
					int destNode = _map->getNodesSize() - 1;
					//_map->setHeuristics(-1);
					vector<GraphNode*> path;
					//if (ai->node == nullptr)
					//{
						int index = 0;
						float closestDistance = 99999999.f;				
						for (int i = 0; i < _map->getNodesSize() - 1; i++)
						{
							float distance = (_map->getNodes()[i]->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y)).length();
							if (distance < closestDistance)
							{
								closestDistance = distance;
								index = i;
							}
						}
						ai->startingNode = _map->getNodes()[index];
					//}
					_map->setHeuristics(ai->startingNode);
					_map->aStarVector(ai->startingNode, _map->getNodes()[destNode], path);

					ai->startingNode->setColour(SDL_Color{ 0,255,0,255 });
					_map->getNodes()[destNode]->setColour(SDL_Color{ 255,0,0,255 });

					string nodePath = "AI :";
					for (int i = 0; i < path.size(); i++)
					{
						nodePath += path[i]->data() + " ,";
					}
					std::cout << nodePath << std::endl;
					ai->path = path;
					ai->callAstar = true;
						
					//}


					
				}
				if (!ai->path.empty())
				{
					TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					helper::Vector2 velocity = ai->path.front()->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
					float distance = (helper::Vector2(transform->rect.x, transform->rect.y) - ai->path.front()->getPosition()).length();
					if (distance < 200.f)
					{
						if (ai->path[0] != _map->getNodes()[_map->getNodesSize() - 1])
						{
							//ai->startingNode = ai->path[0];
							//ai->startingNode->setColour(SDL_Color{ 0,255,0,255 });
							ai->path.erase(ai->path.begin());
						}
					}

					helper::Vector2 dir = velocity.normalize();

					physics->xAcceleration = 0.06f * dir.x;
					physics->yAcceleration = 0.06f * dir.y;


					physics->xVelocity += physics->xAcceleration;
					physics->yVelocity += physics->yAcceleration;
				}

			}
		}
	}
}
