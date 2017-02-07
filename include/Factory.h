#pragma once
#include "Entity.h"
#include "Factory.h"
#include "SpriteComponent.h"
#include "BoundsComponent.h"
#include "SDL_image.h"
class Factory {
public:

	Entity* CreateTile(SDL_Texture* texture, float x, float y, float w, float h)
	{
		Entity* tile = new Entity(Entity::Type::Wall);
		tile->AddComponent(new BoundsComponent(x, y, w, h));
		tile->AddComponent(new SpriteComponent(texture));

		return tile;
	}

	Entity* CreateFlag(SDL_Texture* texture, float x, float y, float w, float h)
	{
		Entity* flag = new Entity(Entity::Type::Flag);
		flag->AddComponent(new BoundsComponent(x, y, w, h));
		flag->AddComponent(new SpriteComponent(texture));

		return flag;
	}
	Entity* CreateCheakpoint(SDL_Texture* texture, float x, float y, float w, float h)
	{
		Entity* checkpoint = new Entity(Entity::Type::Checkpoint);
		checkpoint->AddComponent(new BoundsComponent(x, y, w, h));
		checkpoint->AddComponent(new SpriteComponent(texture));

		return checkpoint;
	}
	Entity* CreateWall(SDL_Texture* texture, float x, float y, float w, float h) 
	{
		Entity* wall = new Entity(Entity::Type::Wall);
		wall->AddComponent(new BoundsComponent(x,y,w,h));
		wall->AddComponent(new SpriteComponent(texture));
		
		return wall;
	}

};