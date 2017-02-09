#include "EntityFactory.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "ControlComponent.h"
#include "BoundsComponent.h"
#include "HealthComponent.h"
#include "SpriteComponent.h"

EntityFactory::EntityFactory(RenderSystem& rs, PhysicsSystem& ps, ControlSystem* cs, std::map<TextureID, SDL_Texture*>& th) : 
	_renderSystem(rs),
	_physicSystem(ps),
	_controlSystem(cs),
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
	player->AddComponent(new SpriteComponent(_textureHolder[TextureID::Player]));
	player->AddComponent(new PhysicsComponent(0,0,0,0));
	player->AddComponent(new ControlComponent());
	//SpriteComponent* sprite = (SpriteComponent*)player->GetComponent(Component::Type::Sprite);
	//player->AddComponent(new BoundsComponent(0,0, sprite->sourceRect.w, sprite->sourceRect.h));
	player->AddComponent(new HealthComponent(100, 100, true));

	_renderSystem.AddEntity(player);
	_physicSystem.AddEntity(player);
	_controlSystem->AddEntity(player);

	return player;
}

Entity* EntityFactory::CreateAIEntity()
{
	Entity* ai = new Entity(Entity::Type::AI);
	ai->AddComponent(new SpriteComponent(_textureHolder[TextureID::Player]));
	ai->AddComponent(new PhysicsComponent(0,0,0,0));
	ai->AddComponent(new HealthComponent(100, 100, true));
	//SpriteComponent* sprite = (SpriteComponent*)ai->GetComponent(Component::Type::Sprite);
	//ai->AddComponent(new BoundsComponent(0,0, sprite->sourceRect.w, sprite->sourceRect.h));

	_renderSystem.AddEntity(ai);
	_physicSystem.AddEntity(ai);

	return ai;
}
Entity* EntityFactory::CreateObstacleEntity()
{
	Entity* obstacle = new Entity(Entity::Type::Obstacle);
	obstacle->AddComponent(new SpriteComponent(_textureHolder[TextureID::EntitySpriteSheet]));
	obstacle->AddComponent(new BoundsComponent());

	_renderSystem.AddEntity(obstacle);

	return obstacle;
}

Entity* EntityFactory::CreatePowerUpEntity()
{
	Entity* powerUp = new Entity(Entity::Type::PowerUp);
	powerUp->AddComponent(new SpriteComponent(_textureHolder[TextureID::EntitySpriteSheet]));
	powerUp->AddComponent(new BoundsComponent());

	_renderSystem.AddEntity(powerUp);

	return powerUp;
}

Entity* EntityFactory::CreateBulletEntity()
{
	Entity* bullet = new Entity(Entity::Type::Bullet);
	bullet->AddComponent(new SpriteComponent(_textureHolder[TextureID::EntitySpriteSheet]));
	bullet->AddComponent(new BoundsComponent());

	_renderSystem.AddEntity(bullet);

	return bullet;
}


Entity* EntityFactory::CreateCheckpointEntity()
{
	Entity* checkpoint = new Entity(Entity::Type::Checkpoint);
	checkpoint->AddComponent(new SpriteComponent(_textureHolder[TextureID::EntitySpriteSheet]));
	checkpoint->AddComponent(new BoundsComponent());

	_renderSystem.AddEntity(checkpoint);

	return checkpoint;
}

Entity* EntityFactory::CreateFlagEntity()
{
	Entity* flag = new Entity(Entity::Type::Flag);
	flag->AddComponent(new SpriteComponent(_textureHolder[TextureID::EntitySpriteSheet]));
	flag->AddComponent(new BoundsComponent());

	_renderSystem.AddEntity(flag);

	return flag;
}

Entity* EntityFactory::CreateTileEntity()
{
	Entity* tile = new Entity(Entity::Type::Tile);
	tile->AddComponent(new SpriteComponent(_textureHolder[TextureID::EntitySpriteSheet]));
	tile->AddComponent(new BoundsComponent());

	_renderSystem.AddEntity(tile);

	return tile;
}