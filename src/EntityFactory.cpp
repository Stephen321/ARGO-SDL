#include "EntityFactory.h"

#include "ColliderComponent.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "GunComponent.h"
#include "AIComponent.h"
#include "DestructionComponent.h"
#include "FlagComponent.h"
#include "CheckpointComponent.h"
#include "StatusEffectComponent.h"
#include "PowerUpComponent.h"
<<<<<<< HEAD
#include "AnimationComponent.h"
=======
#include "WeaponComponent.h"
>>>>>>> master

#include "ConstHolder.h"




EntityFactory::EntityFactory()	
{
}

EntityFactory::~EntityFactory()
{

}


void EntityFactory::Initialize(std::map<TextureID, SDL_Texture*>* th)
{
	_textureHolder = th;
}

Entity* EntityFactory::CreateEntity(EntityType t, int id)
{
	Entity* entity = nullptr;

	switch (t)
	{
	case EntityType::Tile:
		entity = CreateTileEntity(id);
		break;
	case EntityType::Checkpoint:
		entity = CreateCheckpointEntity(id);
		break;
	case EntityType::Bullet:
		entity = CreateBulletEntity(id);
		break;
	case EntityType::PowerUp:
		entity = CreatePowerUpEntity(id);
		break;
	case EntityType::AI:
		entity = CreateAIEntity(id);
		break;
	case EntityType::Player:
		entity = CreatePlayerEntity(id);
		break;
	case EntityType::Weapon:
		entity = CreateWeaponEntity(id);
		break;
	case EntityType::Flag:
		entity = CreateFlagEntity(id);
		break;
	case EntityType::UI:
		entity = CreateUIEntity(id);
		break;

	default:
		break;
	}

	return entity;
}

Entity* EntityFactory::CreatePlayerEntity(int id)
{
	Entity* player = new Entity(EntityType::Player);
	SpriteComponent* spriteComponent= new SpriteComponent((*_textureHolder)[TextureID::Player]);



	player->AddComponent(spriteComponent);
<<<<<<< HEAD
	player->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w / 2, spriteComponent->sourceRect.h / 7));
	player->AddComponent(new HealthComponent(100, 100, true));
=======
	player->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
>>>>>>> master
	player->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	player->AddComponent(new ColliderComponent());
	player->AddComponent(new FlagComponent());
	player->AddComponent(new StatusEffectComponent());
<<<<<<< HEAD
	player->AddComponent(new AnimationComponent());
=======
	player->AddComponent(new WeaponComponent());
>>>>>>> master

	return player;
}
Entity* EntityFactory::CreateAIEntity(int id)
{
	Entity* ai = new Entity(EntityType::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);

	ai->AddComponent(spriteComponent);
<<<<<<< HEAD
	ai->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w / 2, spriteComponent->sourceRect.h / 7));
	ai->AddComponent(new HealthComponent(100, 100, true));
=======
	ai->AddComponent(new TransformComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
>>>>>>> master
	ai->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));
	ai->AddComponent(new ColliderComponent());
	ai->AddComponent(new FlagComponent());
	ai->AddComponent(new AIComponent());
	ai->AddComponent(new StatusEffectComponent());
<<<<<<< HEAD
	ai->AddComponent(new AnimationComponent());
=======
	ai->AddComponent(new WeaponComponent());
>>>>>>> master

	return ai;
}
Entity* EntityFactory::CreatePowerUpEntity(int id)
{
	Entity* powerUp = new Entity(EntityType::PowerUp);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::PowerUp]);
	spriteComponent->sourceRect = { id * 48, 0, 48, 48 };

	powerUp->AddComponent(spriteComponent);
	powerUp->AddComponent(new TransformComponent(0, 0, 48, 48));
	powerUp->AddComponent(new ColliderComponent());
	powerUp->AddComponent(new DestructionComponent());
	powerUp->AddComponent(new PowerUpComponent(static_cast<PowerUpComponent::Type>(id)));

	return powerUp;
}
Entity* EntityFactory::CreateWeaponEntity(int id)
{
	Entity* weapon = new Entity(EntityType::Weapon);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Weapon]);
	spriteComponent->sourceRect = { id * 87, 0, 87, 26 };

	weapon->AddComponent(new TransformComponent(0.f, 0.f, 87, 26, 30, 13, 1.0f, 1.0f, 0));
	weapon->AddComponent(spriteComponent);
	weapon->AddComponent(new GunComponent(FIRE_RATE[id], AMMO[id], id));
	weapon->AddComponent(new DestructionComponent());

	return weapon;
}
Entity* EntityFactory::CreateBulletEntity(int id)
{
	Entity* bullet = new Entity(EntityType::Bullet);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Bullet]);
	spriteComponent->sourceRect = { id * 16, 0, 16, 16 };

	bullet->AddComponent(new TransformComponent(0.f, 0.f, 16, 16, 8, 8, 1.0f, 1.0f, 0));
	bullet->AddComponent(spriteComponent);
	bullet->AddComponent(new PhysicsComponent(0, 0, 0, 0, MAX_BULLET_VELOCITY));
	bullet->AddComponent(new ColliderComponent());
	bullet->AddComponent(new DestructionComponent());

	return bullet;
}
Entity* EntityFactory::CreateCheckpointEntity(int id)
{
	Entity* checkpoint = new Entity(EntityType::Checkpoint);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Checkpoint]);

	checkpoint->AddComponent(spriteComponent);
	checkpoint->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w * 0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	checkpoint->AddComponent(new ColliderComponent());
	checkpoint->AddComponent(new CheckpointComponent(id));

	return checkpoint;
}
Entity* EntityFactory::CreateFlagEntity(int id)
{
	Entity* flag = new Entity(EntityType::Flag);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Flag]);

	flag->AddComponent(spriteComponent);
	flag->AddComponent(new TransformComponent(0.f, 0.f, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h, spriteComponent->sourceRect.w*0.5f, spriteComponent->sourceRect.h*0.5f, 1.0f, 1.0f, 0));
	flag->AddComponent(new ColliderComponent());
	flag->AddComponent(new PhysicsComponent(0, 0, PLAYER_ACCEL_RATE, PLAYER_ACCEL_RATE, MAX_PLAYER_VELOCITY));

	return flag;
}
Entity* EntityFactory::CreateTileEntity(int id)
{
	Entity* tile = new Entity(EntityType::Tile);

	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet]);

	switch (id)
	{
	case 1:
	{
		spriteComponent->sourceRect = { 0, 0, 0, 0 };
		break;
	}
	case 2:
	{
		spriteComponent->sourceRect = { 1, 0, 0, 0 };
		break;
	}
	case 3:
	{
		spriteComponent->sourceRect = { 2, 0, 0, 0 };
		break;
	}
	default:
		break;
	}

	tile->AddComponent(spriteComponent);
	tile->AddComponent(new TransformComponent(0, 0, 0, 0));

	return tile;
}
Entity* EntityFactory::CreateUIEntity(int id)
{
	Entity* ui = new Entity(EntityType::UI);
	ui->AddComponent(new TransformComponent(0, 0, 0, 0));
	ui->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));

	return ui;
}