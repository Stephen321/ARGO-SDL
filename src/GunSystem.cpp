#include "GunSystem.h"
#include "GunComponent.h"
#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "DestructionComponent.h"
#include "PowerUpComponent.h"

#include "ConstHolder.h"


GunSystem::GunSystem(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests, float updateRate)
	: System(updateRate)
	, _creationRequests(creationRequests)
	, _recoil(0)
{
}


GunSystem::~GunSystem()
{
}


void GunSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				GunComponent* gun = static_cast<GunComponent*>(e->GetComponent(Component::Type::Gun));

				if (gun->triggered)
				{//Shoot if triggered
					gun->fireTimer = gun->maxTimer;
					gun->triggered = false;
					gun->canFire = false;
					gun->ammo--;


					//Creates a bullet Request
					CreateBulletRequest(e, gun->id);


					if (gun->ammo == 0)
					{//Destroy weapon if out of ammo
						static_cast<DestructionComponent*>(e->GetComponent(Component::Type::Destroy))->destroy = true;
					}
				}
				else if (!gun->canFire)
				{
					gun->fireTimer -= dt;

					if (gun->fireTimer < 0)
					{
						gun->canFire = true;
					}
				}
			}
		}
	}
}

void GunSystem::CreateBulletRequest(Entity*& e, int id)
{
	TransformComponent* gunTransform = static_cast<TransformComponent*>(e->GetComponent(Component::Type::Transform));

	GunComponent* gun = static_cast<GunComponent*>(e->GetComponent(Component::Type::Gun));

	if (gun->id == (int)PowerUpComponent::Type::Shotgun)
	{
		std::vector<float> data = std::vector<float>();

		float barrelLenght = sqrt(gunTransform->rect.w * gunTransform->rect.w + gunTransform->rect.h * gunTransform->rect.h);

		data.push_back(id); //id
		data.push_back(gunTransform->rect.x + cos(gunTransform->angle * M_PI / 180.0f) * barrelLenght); //xPosition
		data.push_back(gunTransform->rect.y + gunTransform->rect.h + sin(gunTransform->angle * M_PI / 180.0f) * barrelLenght); //yPosition
		data.push_back(-1); //width
		data.push_back(-1); //height
		data.push_back(gunTransform->angle); //angle
		data.push_back(cos((gunTransform->angle + 30) * M_PI / 180.0f) * (MAX_BULLET_VELOCITY)); //xVelocity
		data.push_back(sin((gunTransform->angle + 30) * M_PI / 180.0f) * (MAX_BULLET_VELOCITY)); //yVelocity

		_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::Bullet, data));

		data.clear();

		barrelLenght = sqrt(gunTransform->rect.w * gunTransform->rect.w + gunTransform->rect.h * gunTransform->rect.h);

		data.push_back(id); //id
		data.push_back(gunTransform->rect.x + cos(gunTransform->angle * M_PI / 180.0f) * barrelLenght); //xPosition
		data.push_back(gunTransform->rect.y - gunTransform->rect.h + sin(gunTransform->angle * M_PI / 180.0f) * barrelLenght); //yPosition
		data.push_back(-1); //width
		data.push_back(-1); //height
		data.push_back(gunTransform->angle); //angle
		data.push_back(cos((gunTransform->angle - 30) * M_PI / 180.0f) * (MAX_BULLET_VELOCITY)); //xVelocity
		data.push_back(sin((gunTransform->angle - 30) * M_PI / 180.0f) * (MAX_BULLET_VELOCITY)); //yVelocity

		_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::Bullet, data));
	}

	std::vector<float> data = std::vector<float>();

	float barrelLenght = sqrt(gunTransform->rect.w * gunTransform->rect.w + gunTransform->rect.h * gunTransform->rect.h);
	//rand() % (int)(gunTransform->rect.h + gunTransform->rect.h) - gunTransform->rect.h;
	int offset = 0;

	if (_recoil == 1)
	{
		offset = gunTransform->rect.h;
	}
	else if (_recoil == 2)
	{
		offset = -gunTransform->rect.h;
		_recoil = 0;
	}
	_recoil++;

	data.push_back(id); //id
	data.push_back(gunTransform->rect.x + cos(gunTransform->angle * M_PI / 180.0f) * barrelLenght); //xPosition
	data.push_back(gunTransform->rect.y + offset + sin(gunTransform->angle * M_PI / 180.0f) * barrelLenght); //yPosition
	data.push_back(-1); //width
	data.push_back(-1); //height
	data.push_back(gunTransform->angle); //angle
	data.push_back(cos(gunTransform->angle * M_PI / 180.0f) * (MAX_BULLET_VELOCITY)); //xVelocity
	data.push_back(sin(gunTransform->angle * M_PI / 180.0f) * (MAX_BULLET_VELOCITY)); //yVelocity

	_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::Bullet, data));
}
