#include "CollisionSystem.h"

#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"
#include "StatusEffectComponent.h"
#include "PowerUpComponent.h"
#include "AnimationComponent.h"
#include "WeaponComponent.h"

#include "ConstHolder.h"
#include "Helpers.h"

#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>



CollisionSystem::CollisionSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, std::vector<std::pair<EntityType, std::vector<float>>>& requests, float updateRate)
	: System(updateRate)
	, _interactionSystemEvents(events)
	, _creationRequests(requests)
{
}

CollisionSystem::~CollisionSystem()
{
}


void CollisionSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			for (Entity* e : (*it).second)
			{
				ColliderComponent* collider = static_cast<ColliderComponent*>(e->GetComponent(Component::Type::Collider));

				collider->body->SetActive(collider->setActive);
			}
		}
	}
}

void CollisionSystem::BeginContact(b2Contact* contact)
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	if (bodyAUserData != "Obstacle" && bodyBUserData != "Obstacle")
	{
		Entity* player = nullptr;
		Entity* other = nullptr;

		FindPlayer(contact, player, other);

		if (player != nullptr && other != nullptr)
		{
			if (other->GetType() == EntityType::AI)
			{
				CheckCharacterToCharacterCollision(player, other);
				std::cout << "CHARACTER->CHARACTER: " << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
			}
			else
			{
				CheckCharacterToObjectCollision(player, other);
				std::cout << "CHARACTER->OBJECT: " << player->GetTypeAsString().c_str() << " collided with " << other->GetTypeAsString().c_str() << std::endl;
			}
		}
	}
}

void CollisionSystem::CheckCharacterToObjectCollision(Entity*& player, Entity*& other)
{
	switch (other->GetType())
	{
	case EntityType::Checkpoint:
		CheckCharacterToCheckpointCollision(player, other);
		break;

	case EntityType::PowerUp:
		CheckCharacterToPowerUpCollision(player, other);
		break;

	case EntityType::Bullet:
		CheckCharacterToBulletCollision(player, other);
		break;

	case EntityType::Flag:
		CheckCharacterToFlagCollision(player, other);
		break;
	}
}

void CollisionSystem::CheckCharacterToCheckpointCollision(Entity*& player, Entity*& other)
{
	FlagComponent* flagComponent = static_cast<FlagComponent*>(player->GetComponent(Component::Type::Flag));
	CheckpointComponent* checkpoint = static_cast<CheckpointComponent*>(other->GetComponent(Component::Type::Checkpoint));
	static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider))->checkpointCollision = std::pair<bool, int>(true, checkpoint->id);

	if (flagComponent->hasFlag)
	{
		if (flagComponent->currentCheckpointID + 1 == checkpoint->id)
		{
			flagComponent->currentCheckpointID++;
			flagComponent->totalCheckpoints++;
			_audioManager->PlayFX("Checkpoint");
			if (flagComponent->currentCheckpointID == 4)
			{
				flagComponent->currentCheckpointID = 0;
				flagComponent->currentLap++;
				if (flagComponent->currentLap == 4)
				{
					//WINNER
				}

			}
		}
	}
}
void CollisionSystem::CheckCharacterToPowerUpCollision(Entity*& player, Entity*& other)
{
	PowerUpComponent* powerUp = static_cast<PowerUpComponent*>(other->GetComponent(Component::Type::PowerUp));

	if (powerUp->type == PowerUpComponent::Type::Handgun || powerUp->type == PowerUpComponent::Type::Shotgun || powerUp->type == PowerUpComponent::Type::SMG)
	{
		TransformComponent* powerUpTransform = static_cast<TransformComponent*>(other->GetComponent(Component::Type::Transform));
		WeaponComponent* weapon = static_cast<WeaponComponent*>(player->GetComponent(Component::Type::Weapon));

		if (weapon->hasWeapon && weapon->id == (int)powerUp->type)
		{
			_interactionSystemEvents[InteractionSystemEvent::WeaponAddBullets].push_back(std::pair<Entity*, Entity*>(player, nullptr));
		}
		else
		{
			std::vector<float> data = std::vector<float>();

			data.push_back((int)powerUp->type); //id
			data.push_back(powerUpTransform->rect.x); //xPosition
			data.push_back(powerUpTransform->rect.y); //yPosition

			weapon->fired = false;
			weapon->hasWeapon = true;
			weapon->id = (int)powerUp->type;

			_creationRequests.push_back(std::pair<EntityType, std::vector<float>>(EntityType::Weapon, data));

			_interactionSystemEvents.at(InteractionSystemEvent::WeaponCreated).push_back(std::pair<Entity*, Entity*>(player, nullptr));
		}
	}
	else
	{
		StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(player->GetComponent(Component::Type::StatusEffect));

		switch (powerUp->type)
		{
		case PowerUpComponent::Type::Invisibility:
			statusEffects->invisible = true;
			statusEffects->invisibleTimer = INVISIBLE_MAX_TIMER;
			std::cout << player->GetTypeAsString().c_str() << "INVISIBLE" << std::endl;
			break;
		case PowerUpComponent::Type::Speed:
			statusEffects->speedUp = true;
			statusEffects->speedUpTimer = SPEED_UP_MAX_TIMER;
			std::cout << player->GetTypeAsString().c_str() << "SPEED UP" << std::endl;
			break;
		default:
			break;
		}
	}

	_interactionSystemEvents.at(InteractionSystemEvent::PowerUpDestoyed).push_back(std::pair<Entity*, Entity*>(other, player));
}
void CollisionSystem::CheckCharacterToBulletCollision(Entity*& player, Entity*& other)
{
	StatusEffectComponent* statusEffects = static_cast<StatusEffectComponent*>(player->GetComponent(Component::Type::StatusEffect));

	if (statusEffects->invisible)
	{
		statusEffects->invisible = false;
		statusEffects->invisibleTimer = 0;
	}

	statusEffects->staggered = true;
	statusEffects->staggeredTimer = STAGGER_MAX_TIMER;

	AnimationComponent* animation = static_cast<AnimationComponent*>(player->GetComponent(Component::Type::Animation));
	animation->state = AnimationComponent::State::Staggered;
	_audioManager->PlayFX("Collision");
}
void CollisionSystem::CheckCharacterToFlagCollision(Entity*& player, Entity*& other)
{
	if (!static_cast<StatusEffectComponent*>(player->GetComponent(Component::Type::StatusEffect))->staggered)
	{
		static_cast<ColliderComponent*>(other->GetComponent(Component::Type::Collider))->setActive = false;
		PhysicsComponent* flagPhysicsComponent = static_cast<PhysicsComponent*>(other->GetComponent(Component::Type::Physics));
		flagPhysicsComponent->xVelocity = 0;
		flagPhysicsComponent->yVelocity = 0;

		FlagComponent* flagComponent = static_cast<FlagComponent*>(player->GetComponent(Component::Type::Flag));
		flagComponent->hasFlag = true;

		ColliderComponent* collider = static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider));

		if (collider->checkpointCollision.first)
		{
			if (flagComponent->currentCheckpointID + 1 == collider->checkpointCollision.second)
			{
				flagComponent->currentCheckpointID++;
				flagComponent->totalCheckpoints++;
				_audioManager->PlayFX("Checkpoint");
				if (flagComponent->currentCheckpointID == 4)
				{
					flagComponent->currentCheckpointID = 0;
					flagComponent->currentLap++;
				}
			}

			collider->checkpointCollision = std::pair<bool, int>(false, -1);
		}

		_interactionSystemEvents.at(InteractionSystemEvent::FlagPicked).push_back(std::pair<Entity*, Entity*>(player, other));
	}
}


void CollisionSystem::CheckCharacterToCharacterCollision(Entity*& player, Entity*& other)
{
	FlagComponent* pFlag = static_cast<FlagComponent*>(player->GetComponent(Component::Type::Flag));
	FlagComponent* oFlag = static_cast<FlagComponent*>(other->GetComponent(Component::Type::Flag)); 

	StatusEffectComponent* playerStatusEffects = static_cast<StatusEffectComponent*>(player->GetComponent(Component::Type::StatusEffect));
	StatusEffectComponent* otherStatusEffects = static_cast<StatusEffectComponent*>(other->GetComponent(Component::Type::StatusEffect));

	if (pFlag->hasFlag)
	{
		playerStatusEffects->staggered = true;
		playerStatusEffects->staggeredTimer = STAGGER_MAX_TIMER;

		_interactionSystemEvents.at(InteractionSystemEvent::FlagDropped).push_back(std::pair<Entity*, Entity*>(player, other));
	}
	else if (oFlag->hasFlag)
	{
		otherStatusEffects->staggered = true;
		otherStatusEffects->staggeredTimer = STAGGER_MAX_TIMER;

		_interactionSystemEvents.at(InteractionSystemEvent::FlagDropped).push_back(std::pair<Entity*, Entity*>(other, player));
	}

	if (playerStatusEffects->invisible)
	{
		playerStatusEffects->invisible = false;
		playerStatusEffects->invisibleTimer = 0;
	}
	if (otherStatusEffects->invisible)
	{
		otherStatusEffects->invisible = false;
		otherStatusEffects->invisibleTimer = 0;
	}

	AnimationComponent* animation = static_cast<AnimationComponent*>(player->GetComponent(Component::Type::Animation));
	animation->state = AnimationComponent::State::Bumping;
	_audioManager->PlayFX("Collision");
}

void CollisionSystem::EndContact(b2Contact* contact)
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	if (bodyAUserData != "Obstacle" && bodyBUserData != "Obstacle")
	{
		Entity* player = nullptr;
		Entity* other = nullptr;

		FindPlayer(contact, player, other);

		if (player != nullptr && other != nullptr)
		{
			if (other->GetType() == EntityType::Checkpoint)
			{
				static_cast<ColliderComponent*>(player->GetComponent(Component::Type::Collider))->checkpointCollision = std::pair<bool, int>(false, -1);
			}
		}
	}
}



void CollisionSystem::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	if (player != nullptr && other != nullptr)
	{
		EntityBounce(worldManifold, player, other);
		
	}
	else if (player != nullptr)
	{
		CharacterObstacleBounce(worldManifold, player);
	}
}

void CollisionSystem::EntityBounce(b2WorldManifold& worldManifold, Entity*& e1, Entity*& e2)
{
	PhysicsComponent* e1Physics = static_cast<PhysicsComponent*>(e1->GetComponent(Component::Type::Physics));
	PhysicsComponent* e2Physics = static_cast<PhysicsComponent*>(e2->GetComponent(Component::Type::Physics));


	float e1speed = sqrt(e1Physics->xVelocity * e1Physics->xVelocity + e1Physics->yVelocity * e1Physics->yVelocity);
	float e2speed = sqrt(e2Physics->xVelocity * e2Physics->xVelocity + e2Physics->yVelocity * e2Physics->yVelocity);

	float maxVelocity = e1Physics->maxVelocity * 0.1f;

	if (e1speed > maxVelocity && e2speed > maxVelocity)
	{
		float xVelocityTemp = e1Physics->xVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;
		float yVelocityTemp = e1Physics->yVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;

		e1Physics->xVelocity = e2Physics->xVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;
		e1Physics->yVelocity = e2Physics->yVelocity * PLAYER_HEAD_ON_PLAYER_RESTITUTION;

		e2Physics->xVelocity = xVelocityTemp;
		e2Physics->yVelocity = yVelocityTemp;
	}
	else
	{
		if (e1speed > maxVelocity)
		{
			e2Physics->xVelocity = e1Physics->xVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;
			e2Physics->yVelocity = e1Physics->yVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;

			e1Physics->xVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
			e1Physics->yVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
		}
		else if (e2speed > maxVelocity)
		{
			e1Physics->xVelocity = e2Physics->xVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;
			e1Physics->yVelocity = e2Physics->yVelocity * PLAYER_STATIONARY_PLAYER_RESTITUTION;

			e2Physics->xVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
			e2Physics->yVelocity *= (1 - PLAYER_STATIONARY_PLAYER_RESTITUTION);
		}
	}

	static_cast<ColliderComponent*>(e1->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(e1Physics->xVelocity*2.f, e1Physics->yVelocity*2.f));
	static_cast<ColliderComponent*>(e2->GetComponent(Component::Type::Collider))->body->SetLinearVelocity(b2Vec2(e2Physics->xVelocity*2.f, e2Physics->yVelocity*2.f));
}

void CollisionSystem::CharacterObstacleBounce(b2WorldManifold& worldManifold, Entity*& player)
{
	PhysicsComponent* physics = static_cast<PhysicsComponent*>(player->GetComponent(Component::Type::Physics));

	float speed = sqrt(physics->xVelocity * physics->xVelocity + physics->yVelocity * physics->yVelocity);

	if (speed != 0)
	{
		float xDirection = physics->xVelocity / speed;
		float yDirection = physics->yVelocity / speed;

		float xDirectionNew = worldManifold.normal.x + xDirection;
		float yDirectionNew = worldManifold.normal.y + yDirection;

		float directionlenght = sqrt(xDirectionNew * xDirectionNew + yDirectionNew * yDirectionNew);

		if (directionlenght != 0)
		{
			speed *= PLAYER_WALL_RESTITUTION;
			physics->xVelocity = (xDirectionNew / directionlenght) * speed;
			physics->yVelocity = (yDirectionNew / directionlenght) * speed;
		}
		else
		{
			physics->xVelocity = -physics->xVelocity * PLAYER_WALL_RESTITUTION;
			physics->yVelocity = -physics->yVelocity * PLAYER_WALL_RESTITUTION;
		}
	}
}

void CollisionSystem::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{
	/*Entity* player = nullptr;
	Entity* other = nullptr;

	FindPlayer(contact, player, other);
	if (player != nullptr && other != nullptr)
		std::cout << player->GetTypeAsString().c_str() << " post solve with " << other->GetTypeAsString().c_str() << std::endl;*/
}

void CollisionSystem::FindPlayer(b2Contact * contact, Entity *& player, Entity *& other)
{
	if (contact->GetFixtureA()->GetBody()->GetUserData() != "Obstacle" && contact->GetFixtureB()->GetBody()->GetUserData() != "Obstacle")
	{
		Entity* a = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
		Entity* b = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (a->GetType() == EntityType::Player)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::Player)
		{
			player = b;
			other = a;
		}
		else if (a->GetType() == EntityType::AI)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::AI)
		{
			player = b;
			other = a;
		}
		else if (a->GetType() == EntityType::Flag)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::Flag)
		{
			player = b;
			other = a;
		}
		else if (a->GetType() == EntityType::Bullet)
		{
			player = a;
			other = b;
		}
		else if (b->GetType() == EntityType::Bullet)
		{
			player = b;
			other = a;
		}
	}
	else
	{
		if (contact->GetFixtureA()->GetBody()->GetUserData() == "Obstacle")
		{
			player = static_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

			// Wall Collision
			if (player->GetType() == EntityType::Player || player->GetType() == EntityType::AI)
			{
				AnimationComponent* animation = static_cast<AnimationComponent*>(player->GetComponent(Component::Type::Animation));
				animation->state = AnimationComponent::State::Bumping;
				_audioManager->PlayFX("Collision");
			}
		}
		else
		{
			player = static_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());

			// Wall Collision
			if (player->GetType() == EntityType::Player || player->GetType() == EntityType::AI)
			{
				AnimationComponent* animation = static_cast<AnimationComponent*>(player->GetComponent(Component::Type::Animation));
				animation->state = AnimationComponent::State::Bumping;
				_audioManager->PlayFX("Collision");
			}
		}
	}
}

