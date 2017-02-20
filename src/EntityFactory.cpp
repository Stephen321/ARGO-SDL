#include "EntityFactory.h"
#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "GunComponent.h"
#include "DestructionComponent.h"
#include "ConstHolder.h"



EntityFactory::EntityFactory()	
{
}

EntityFactory::~EntityFactory()
{

}


void EntityFactory::Initialize(SystemManager* sm, std::map<TextureID, SDL_Texture*>* th)
{
	_systemManager = sm;

	_textureHolder = th;
}

Entity* EntityFactory::CreateEntity(EntityType t)
{
	Entity* entity = nullptr;

	switch (t)
	{
	case EntityType::Tile:
		entity = CreateTileEntity();
		break;
	case EntityType::Checkpoint:
		entity = CreateCheckpointEntity();
		break;
	case EntityType::Obstacle:
		entity = CreateObstacleEntity();
		break;
	case EntityType::Bullet:
		entity = CreateBulletEntity();
		break;
	case EntityType::Point:
		entity = CreatePointEntity();
		break;
	case EntityType::PowerUp:
		entity = CreatePowerUpEntity();
		break;
	case EntityType::AI:
		entity = CreateAIEntity();
		break;
	case EntityType::Player:
		entity = CreatePlayerEntity();
		break;
	case EntityType::Weapon:
		entity = CreateWeaponEntity();
		break;
	case EntityType::Flag:
		entity = CreateFlagEntity();
		break;

	default:
		break;
	}

	return entity;
}

Entity* EntityFactory::CreatePlayerEntity()
{
	Entity* player = new Entity(EntityType::Player);
	SpriteComponent* spriteComponent= new SpriteComponent((*_textureHolder)[TextureID::Player]);

	player->AddComponent(spriteComponent);
	player->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	player->AddComponent(new HealthComponent(100, 100, true));
	player->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	player->AddComponent(new ColliderComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, player);
	_systemManager->AddEntity(SystemManager::SystemType::Physics, player);
	_systemManager->AddEntity(SystemManager::SystemType::Camera, player);

	return player;
}

Entity* EntityFactory::CreateAIEntity()
{
	Entity* ai = new Entity(EntityType::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);
	ai->AddComponent(spriteComponent);
	ai->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	ai->AddComponent(new HealthComponent(100, 100, true));
	ai->AddComponent(new PhysicsComponent(0, 0, 0, 0, 10));
	ai->AddComponent(new ColliderComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, ai);
	_systemManager->AddEntity(SystemManager::SystemType::Physics, ai);

	return ai;
}
Entity* EntityFactory::CreateObstacleEntity()
{

	Entity* obstacle = new Entity(EntityType::Obstacle);
	obstacle->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	obstacle->AddComponent(new TransformComponent());
	obstacle->AddComponent(new ColliderComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, obstacle);

	return obstacle;
}

Entity* EntityFactory::CreatePowerUpEntity()
{
	Entity* powerUp = new Entity(EntityType::PowerUp);
	powerUp->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	powerUp->AddComponent(new TransformComponent());
	powerUp->AddComponent(new ColliderComponent());
	powerUp->AddComponent(new DestructionComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, powerUp);
	_systemManager->AddEntity(SystemManager::SystemType::Destruction, powerUp);

	return powerUp;
}

Entity* EntityFactory::CreateWeaponEntity()
{
	Entity* weapon = new Entity(EntityType::Weapon);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Weapon]);

	weapon->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.25f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	weapon->AddComponent(spriteComponent);
	weapon->AddComponent(new GunComponent(BULLET_FIRE_RATE, BULLET_AMMO));
	weapon->AddComponent(new DestructionComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, weapon);
	_systemManager->AddEntity(SystemManager::SystemType::Gun, weapon);
	_systemManager->AddEntity(SystemManager::SystemType::Destruction, weapon);

	return weapon;
}
Entity* EntityFactory::CreateBulletEntity()
{
	Entity* bullet = new Entity(EntityType::Bullet);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Bullet]);

	bullet->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	bullet->AddComponent(spriteComponent);
	bullet->AddComponent(new PhysicsComponent(0, 0, 0, 0, MAX_BULLET_VELOCITY));
	bullet->AddComponent(new ColliderComponent());
	bullet->AddComponent(new DestructionComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, bullet);
	_systemManager->AddEntity(SystemManager::SystemType::Physics, bullet);
	_systemManager->AddEntity(SystemManager::SystemType::Destruction, bullet);

	return bullet;
}


Entity* EntityFactory::CreateCheckpointEntity()
{
	Entity* checkpoint = new Entity(EntityType::Checkpoint);
	checkpoint->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	checkpoint->AddComponent(new TransformComponent());
	checkpoint->AddComponent(new ColliderComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, checkpoint);

	return checkpoint;
}

Entity* EntityFactory::CreateFlagEntity()
{
	Entity* flag = new Entity(EntityType::Flag);
	flag->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	flag->AddComponent(new TransformComponent());
	flag->AddComponent(new ColliderComponent());


	_systemManager->AddEntity(SystemManager::SystemType::Render, flag);

	return flag;
}

Entity* EntityFactory::CreateTileEntity()
{
	Entity* tile = new Entity(EntityType::Tile);
	tile->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet]));
	tile->AddComponent(new TransformComponent());

	_systemManager->AddEntity(SystemManager::SystemType::Render, tile);

	return tile;
}

Entity* EntityFactory::CreatePointEntity()
{
	Entity* point = new Entity(EntityType::Point);
	point->AddComponent(new TransformComponent());
	//_controlSystem->AddEntity(player);

	return point;
}