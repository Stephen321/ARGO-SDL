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

void AISystem::Initialize(Graph<string>* map)
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


					if (!ai->callAstar)
					{
						int originNode = 1;
						_map->nodeArray()[originNode]->setColour(SDL_Color{ 0,255,0,255 });

						int destNode = 61;
						_map->nodeArray()[destNode]->setColour(SDL_Color{ 255,0,0,255 });
						_map->setHeuristics(_map->nodeArray()[destNode]);
						vector<Node*> path;
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

						//std::cout << "xAccerleration : " << 
						std::cout << "xVelocity : " << physics->xVelocity << endl;
						//std::cout << "yVel : " << physics->yVelocity;
					}

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
