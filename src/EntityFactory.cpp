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
	default://flag
		entity = CreateFlagEntity();
		break;
	}
	return entity;
}

Entity* EntityFactory::CreatePlayerEntity()
{
	Entity* player = new Entity(Entity::Type::Player);
	SpriteComponent* spriteComponent= new SpriteComponent((*_textureHolder)[TextureID::Player]);
	player->AddComponent(spriteComponent);
	player->AddComponent(new BoundsComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	player->AddComponent(new HealthComponent(100, 100, true));
	player->AddComponent(new PhysicsComponent(0, 0, 1.f, 1.f));
	player->AddComponent(new ControlComponent());
	player->AddComponent(new CollisionComponent(nullptr));
	_renderSystem->AddEntity(player);
	_physicSystem->AddEntity(player);
	_cameraSystem->AddEntity(player);
	//_controlSystem->AddEntity(player);

	return player;
}

Entity* EntityFactory::CreateAIEntity()
{
	Entity* ai = new Entity(Entity::Type::AI);
	SpriteComponent* spriteComponent = new SpriteComponent((*_textureHolder)[TextureID::Player]);
	ai->AddComponent(spriteComponent);
	ai->AddComponent(new BoundsComponent(0, 0, spriteComponent->sourceRect.w, spriteComponent->sourceRect.h));
	ai->AddComponent(new HealthComponent(100, 100, true));
	ai->AddComponent(new PhysicsComponent(0, 0, 0, 0));
	

	_renderSystem->AddEntity(ai);
	_physicSystem->AddEntity(ai);

	return ai;
}
Entity* EntityFactory::CreateObstacleEntity()
{
	Entity* obstacle = new Entity(Entity::Type::Obstacle);
	obstacle->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	obstacle->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(obstacle);

	return obstacle;
}

Entity* EntityFactory::CreatePowerUpEntity()
{
	Entity* powerUp = new Entity(Entity::Type::PowerUp);
	powerUp->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	powerUp->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(powerUp);

	return powerUp;
}

Entity* EntityFactory::CreateBulletEntity()
{
	Entity* bullet = new Entity(Entity::Type::Bullet);
	bullet->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	bullet->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(bullet);

	return bullet;
}


Entity* EntityFactory::CreateCheckpointEntity()
{
	Entity* checkpoint = new Entity(Entity::Type::Checkpoint);
	checkpoint->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	checkpoint->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(checkpoint);

	return checkpoint;
}

Entity* EntityFactory::CreateFlagEntity()
{
	Entity* flag = new Entity(Entity::Type::Flag);
	flag->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::EntitySpriteSheet]));
	flag->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(flag);

	return flag;
}

Entity* EntityFactory::CreateTileEntity()
{
	Entity* tile = new Entity(Entity::Type::Tile);
	tile->AddComponent(new SpriteComponent((*_textureHolder)[TextureID::TilemapSpriteSheet]));
	tile->AddComponent(new BoundsComponent());

	_renderSystem->AddEntity(tile);

	return tile;
}