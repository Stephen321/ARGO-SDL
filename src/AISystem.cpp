#include "AISystem.h"
#include "AIComponent.h"

AISystem::AISystem(Graph<string, int, int>* map)
{
	_map = map;
}

AISystem::~AISystem()
{

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
				float updateTimer = ai->updateTimer;
				updateTimer += dt;
				if (updateTimer> updateRate)
				{
					updateTimer -= updateRate;

					int originNode = 1;
					_map->nodeArray()[originNode]->setColour(SDL_Color{ 0,255,0,255 });

					int destNode = 60;
					_map->nodeArray()[destNode]->setColour(SDL_Color{ 255,0,0,255 });
					_map->setHeuristics(_map->nodeArray()[destNode]);
					vector<Node*> path;
					_map->aStar(_map->nodeArray()[originNode], _map->nodeArray()[destNode], path);
					_map->nodeArray()[originNode]->setColour(SDL_Color{ 0,255,0,255 });
					_map->nodeArray()[destNode]->setColour(SDL_Color{ 255,0,0,255 });

					/*
					PhysicsComponent* physics = static_cast<PhysicsComponent*>(e->GetComponent(Component::Type::Physics));
					BoundsComponent* bounds = static_cast<BoundsComponent*>(e->GetComponent(Component::Type::Bounds));
					CollisionComponent* collision = static_cast<CollisionComponent*>(e->GetComponent(Component::Type::Collider));
					
					float maxVelocity = physics->maxVelocity;

					float xDrag = (physics->xDir == 0) ? -physics->xVelocity * DRAG : 0.f;
					float yDrag = (physics->yDir == 0) ? -physics->yVelocity * DRAG : 0.f;

					physics->xVelocity += (xDrag + (physics->xDir * physics->xAcceleration)) * dt;//change dt to _updateRate?
					physics->yVelocity += (yDrag + (physics->yDir * physics->yAcceleration)) * dt;

					float currentVelocity = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);
					if (currentVelocity > maxVelocity)
					{
						physics->xVelocity = (physics->xVelocity / currentVelocity) * maxVelocity;
						physics->yVelocity = (physics->yVelocity / currentVelocity) * maxVelocity;
					}

					if (physics->xDir == 0 && std::abs(physics->xVelocity) <= 0.1f) { physics->xVelocity = 0.f; }
					if (physics->yDir == 0 && std::abs(physics->yVelocity) <= 0.1f) { physics->yVelocity = 0.f; }

					collision->body->SetLinearVelocity(b2Vec2(physics->xVelocity, physics->yVelocity));

					bounds->rect.x = (int)metersToPixels(collision->body->GetPosition().x) - bounds->rect.w * 0.5f;
					bounds->rect.y = (int)metersToPixels(collision->body->GetPosition().y) - bounds->rect.h * 0.5f;

					physics->xDir = 0;
					physics->yDir = 0;
					*/
				}

			
				//std::cout << "Velcoity.x: " << collision->body->GetLinearVelocity().x << " y: " << collision->body->GetLinearVelocity().y << std::endl;
			}
		}
	}
}
