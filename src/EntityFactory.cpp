#include "EntityFactory.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "ControlComponent.h"
#include "BoundsComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"

EntityFactory::EntityFactory(RenderSystem* rs, PhysicsSystem* ps, ControlSystem* ctls, CameraSystem* cs, std::map<TextureID, SDL_Texture*>* th) : 
	_renderSystem(rs),
	_cameraSystem(cs),
	_physicSystem(ps),
	_controlSystem(ctls),
	_textureHolder(th)
{
}

EntityFactory::~EntityFactory()
{

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
	default://flag
		entity = CreateFlagEntity();
		break;
	}
	return entity;
}

Entity* EntityFactory::CreatePlayerEntity()
{
	Entity* player = new Entity(EntityType::Player);
	SpriteComponent* spriteComponent= new SpriteComponent((*_textureHolder)[TextureID::Player]);
	player->AddComponent(spriteComponent);
	player->AddComponent(new BoundsComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	player->AddComponent(new HealthComponent(100, 100, true));
	player->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	player->AddComponent(new ControlComponent());
	player->AddComponent(new CollisionComponent());
	_renderSystem->AddEntity(player);
	_physicSystem->AddEntity(player);
	_cameraSystem->AddEntity(player);
	//_controlSystem->AddEntity(player);

	return player;
}

Entity* EntityFactory::CreateAIEntity()
{
	Entity* ai = new Entity(EntityType::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);
	ai->AddComponent(spriteComponent);
	ai->AddComponent(new BoundsComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	ai->AddComponent(new HealthComponent(100, 100, true));
	ai->AddComponent(new PhysicsComponent(0, 0, 0, 0, 10));
	ai->AddComponent(new CollisionComponent());

	_renderSystem->AddEntity(ai);
	_physicSystem->AddEntity(ai);

	return ai;
}
Entity* EntityFactory::CreateObstacleEntity()
{
	Entity* obstacle = new Entity(EntityType::Obstacle);
	obstacle->AddComponent(new BoundsComponent());
	obstacle->AddComponent(new CollisionComponent());

	return obstacle;
}

Entity* EntityFactory::CreatePowerUpEntity()
{
	Entity* powerUp = new Entity(EntityType::PowerUp);
	powerUp->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	powerUp->AddComponent(new BoundsComponent());
	powerUp->AddComponent(new CollisionComponent());

	_renderSystem->AddEntity(powerUp);

	return powerUp;
}

Entity* EntityFactory::CreateBulletEntity()
{
	Entity* bullet = new Entity(EntityType::Bullet);
	bullet->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	bullet->AddComponent(new BoundsComponent());
	bullet->AddComponent(new CollisionComponent());

	_renderSystem->AddEntity(bullet);

	return bullet;
}


Entity* EntityFactory::CreateCheckpointEntity()
{
	Entity* checkpoint = new Entity(EntityType::Checkpoint);
	checkpoint->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	checkpoint->AddComponent(new BoundsComponent());
	checkpoint->AddComponent(new CollisionComponent());

	_renderSystem->AddEntity(checkpoint);

	return checkpoint;
}

Entity* EntityFactory::CreateFlagEntity()
{
	Entity* flag = new Entity(EntityType::Flag);
	flag->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	flag->AddComponent(new BoundsComponent());
	flag->AddComponent(new CollisionComponent());

	_renderSystem->AddEntity(flag);

	return flag;
}

Entity* EntityFactory::CreateTileEntity()
{
	Entity* tile = new Entity(EntityType::Tile);
	tile->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet]));
	tile->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(tile);

	return tile;
}

Entity* EntityFactory::CreatePointEntity()
{
	Entity* point = new Entity(EntityType::Point);
	point->AddComponent(new BoundsComponent());
	//_controlSystem->AddEntity(player);

	return point;
}