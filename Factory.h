#pragma once
#include "Entity.h"
#include "Factory.h"
#include "SpriteComponent.h"
#include "BoundsComponent.h"
#include "SDL_image.h"
class Factory {
public:

	Entity* CreateTile(SDL_Texture* texture,float x,float y, float w, float h)
	{
		Entity* tile = new Entity(Entity::Type::Tile);
		tile->AddComponent(new SpriteComponent(texture));
		tile->AddComponent(new BoundsComponent(x, y, w, h));
		return tile;
	}
	Entity* CreateWall(SDL_Texture* texture, float x, float y, float w, float h)
	{
		Entity* wall = new Entity(Entity::Type::Wall);
		wall->AddComponent(new SpriteComponent(texture));
		wall->AddComponent(new BoundsComponent(x, y, w, h));
		return wall;
	}

};