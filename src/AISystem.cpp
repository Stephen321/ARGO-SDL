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
				float updateRate = ai->updateRate;
				ai->updateTimer += dt;
				if (ai->updateTimer > updateRate)
				{
				
				
					ai->updateTimer -= updateRate;


					if (!ai->callAstar)//testing
					{
						int originNode = 0;
						_map->nodeArray()[originNode]->setColour(SDL_Color{ 0,255,0,255 });

						int destNode = 25;
						_map->nodeArray()[destNode]->setColour(SDL_Color{ 255,0,0,255 });
						
						vector<GraphNode*> path;
						_map->aStar(_map->nodeArray()[originNode], _map->nodeArray()[destNode], path);
						_map->nodeArray()[originNode]->setColour(SDL_Color{ 0,255,0,255 });
						_map->nodeArray()[destNode]->setColour(SDL_Color{ 255,0,0,255 });

						ai->path = path;
						ai->callAstar = true;
					}


					if (!ai->path.empty())
					{
						TransformComponent* transform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));
						PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
						helper::Vector2 velocity = ai->path.front()->getPosition() - helper::Vector2(transform->rect.x, transform->rect.y);
						float distance = (helper::Vector2(transform->rect.x, transform->rect.y) - ai->path.front()->getPosition()).length();
						if (distance < 90.f)
						{
							ai->path.erase(ai->path.begin());
						}
		
						helper::Vector2 dir = velocity.normalize();

						physics->xAcceleration = 0.06f * dir.x;
						physics->yAcceleration = 0.06f * dir.y;


						physics->xVelocity += physics->xAcceleration ;
						physics->yVelocity += physics->yAcceleration ;
					}
				}

			}
		}
	}
}
