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

void AISystem::Initialize(Graph* waypoints)
{
	_waypoints = waypoints;
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

					_waypoints->reset();
					int destNode = _waypoints->getNodesSize() - 1;
					vector<GraphNode*> path;
	
					int index = 0;
					float closestDistance = 99999999.f;				
					for (int i = 0; i < _waypoints->getNodesSize() - 1; i++)
					{
						float distance = (_waypoints->getNodes()[i]->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y)).length();
						if (distance < closestDistance)
						{
							closestDistance = distance;
							index = i;
						}
					}
					ai->startingNode = _waypoints->getNodes()[index];
					
					_waypoints->setHeuristics(ai->startingNode);
					_waypoints->aStar(ai->startingNode, _waypoints->getNodes()[destNode], path);

					ai->startingNode->setColour(SDL_Color{ 0,255,0,255 });
					_waypoints->getNodes()[destNode]->setColour(SDL_Color{ 255,0,0,255 });

					string nodePath = "AI :";
					for (int i = 0; i < path.size(); i++)
					{
						nodePath += path[i]->data() + " ,";
					}
					std::cout << nodePath << std::endl;
					ai->path = path;
	
				}
				if (!ai->path.empty())
				{
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					helper::Vector2 velocity = ai->path.front()->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
					float distance = helper::Vector2(transform->rect.x, transform->rect.y).distance(ai->path.front()->getPosition());
					if (distance < AI_DETECTION_RADIUS)
					{
						if (ai->path[0] != _waypoints->getNodes()[_waypoints->getNodesSize() - 1])
						{
							ai->path.erase(ai->path.begin());
						}
					}

					helper::Vector2 dir = velocity.normalize();
					//physics->xDir = dir.x;
					//physics->yDir = dir.y;
					/*physics->xAcceleration = 0.06f;
					physics->yAcceleration = 0.06f;
					physics->xVelocity += physics->xAcceleration * dir.x;// *dt;
					physics->yVelocity += physics->yAcceleration * dir.y;// *dt;*/
				}

			}
		}
	}
}
