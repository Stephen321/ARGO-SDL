#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <map>

#include "Entity.h"
#include "SDL_image.h"
#include "ResourceIdentifier.h"


class SystemManager;

class EntityFactory
{
public: 
										EntityFactory();
										~EntityFactory();

	void								Initialize(SystemManager* sm, std::map<TextureID, SDL_Texture*>* th);

	Entity*								CreateEntity(EntityType t);

private:
	Entity*								CreateWeaponEntity();
	Entity*								CreatePlayerEntity();
	Entity*								CreateAIEntity();
	Entity*								CreateObstacleEntity();
	Entity*								CreatePowerUpEntity();
	Entity*								CreateBulletEntity();
	Entity*								CreateCheckpointEntity();
	Entity*								CreateFlagEntity();
	Entity*								CreateTileEntity();
	Entity*								CreatePointEntity();

private:
	std::map<TextureID, SDL_Texture*>*	_textureHolder;

	SystemManager*						_systemManager;
};

#include "SystemManager.h"

#endif