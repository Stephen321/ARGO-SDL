#include "EntityFactory.h"
#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "ControlComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"
#include "GunComponent.h"



EntityFactory::EntityFactory(RenderSystem* rs, PhysicsSystem* ps, ControlSystem* ctls, CameraSystem* cs, GunSystem* gs, FiringSystem* fs, std::map<TextureID, SDL_Texture*>* th) :
	_renderSystem(rs),
	_cameraSystem(cs),
	_physicSystem(ps),
	_controlSystem(ctls),
	_gunSystem(gs),
	_firingSystem(fs),
	_textureHolder(th)
{
}

EntityFactory::~EntityFactory()
{

}


Entity* EntityFactory::CreateEntity(Entity::Type t)
{
	Entity* entity = nullptr;

	switch (t)
	{
	case Entity::Type::Tile:
		entity = CreateTileEntity();
		break;
	case Entity::Type::Checkpoint:
		entity = CreateCheckpointEntity();
		break;
	case Entity::Type::Obstacle:
		entity = CreateObstacleEntity();
		break;
	case Entity::Type::Bullet:
		entity = CreateBulletEntity();
		break;
	case Entity::Type::PowerUp:
		entity = CreatePowerUpEntity();
		break;
	case Entity::Type::AI:
		entity = CreateAIEntity();
		break;
	case Entity::Type::Player:
		entity = CreatePlayerEntity();
		break;
	case Entity::Type::Weapon:
		entity = CreateWeaponEntity();
		break;
	case Entity::Type::Flag:
		entity = CreateFlagEntity();
		break;
	default:
		break;
	}

	return entity;
}

Entity* EntityFactory::CreatePlayerEntity()
{
	Entity* player = new Entity(Entity::Type::Player);
	SpriteComponent* spriteComponent= new SpriteComponent((*_textureHolder)[TextureID::Player]);

	player->AddComponent(spriteComponent);
	player->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	player->AddComponent(new HealthComponent(100, 100, true));
	player->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	player->AddComponent(new ControlComponent());
	player->AddComponent(new ColliderComponent(nullptr));

	_renderSystem->AddEntity(player);
	_physicSystem->AddEntity(player);
	_cameraSystem->AddEntity(player);

	return player;
}

Entity* EntityFactory::CreateAIEntity()
{
	Entity* ai = new Entity(Entity::Type::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);
	ai->AddComponent(spriteComponent);
	ai->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	ai->AddComponent(new HealthComponent(100, 100, true));
	ai->AddComponent(new PhysicsComponent(0, 0, 0, 0, 10));

	_renderSystem->AddEntity(ai);
	_physicSystem->AddEntity(ai);

	return ai;
}
Entity* EntityFactory::CreateObstacleEntity()
{
	Entity* obstacle = new Entity(Entity::Type::Obstacle);
	obstacle->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	obstacle->AddComponent(new TransformComponent());

	_renderSystem->AddEntity(obstacle);

	return obstacle;
}

Entity* EntityFactory::CreatePowerUpEntity()
{
	Entity* powerUp = new Entity(Entity::Type::PowerUp);
	powerUp->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	powerUp->AddComponent(new TransformComponent());

	_renderSystem->AddEntity(powerUp);

	return powerUp;
}

Entity* EntityFactory::CreateWeaponEntity()
{
	Entity* weapon = new Entity(Entity::Type::Weapon);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Weapon]);

	weapon->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.25f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	weapon->AddComponent(spriteComponent);
	weapon->AddComponent(new GunComponent(BULLET_FIRE_RATE, BULLET_AMMO));

	_renderSystem->AddEntity(weapon);
	_controlSystem->AddTurret(weapon);
	_gunSystem->AddEntity(weapon);
	_firingSystem->AddEntity(weapon);

	return weapon;
}
Entity* EntityFactory::CreateBulletEntity()
{
	Entity* bullet = new Entity(Entity::Type::Bullet);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Bullet]);

	bullet->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	bullet->AddComponent(spriteComponent);
	bullet->AddComponent(new PhysicsComponent(0, 0, 0, 0, MAX_BULLET_VELOCITY));
	bullet->AddComponent(new ColliderComponent(nullptr));

	_renderSystem->AddEntity(bullet);
	_physicSystem->AddEntity(bullet);

	return bullet;
}


Entity* EntityFactory::CreateCheckpointEntity()
{
	Entity* checkpoint = new Entity(Entity::Type::Checkpoint);
	checkpoint->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	checkpoint->AddComponent(new TransformComponent());

	_renderSystem->AddEntity(checkpoint);

	return checkpoint;
}

Entity* EntityFactory::CreateFlagEntity()
{
	Entity* flag = new Entity(Entity::Type::Flag);
	flag->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	flag->AddComponent(new TransformComponent());

	_renderSystem->AddEntity(flag);

	return flag;
}

Entity* EntityFactory::CreateTileEntity()
{
	Entity* tile = new Entity(Entity::Type::Tile);
	tile->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet]));
	tile->AddComponent(new TransformComponent());

	_renderSystem->AddEntity(tile);

	return tile;
}