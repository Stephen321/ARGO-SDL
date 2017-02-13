#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <map>
#include "Entity.h"
#include "SDL_image.h"
#include "ResourceIdentifier.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "ControlSystem.h"
#include "CameraSystem.h"
#include "GunSystem.h"
#include "FiringSystem.h"


class FiringSystem;

class EntityFactory
{
public: 
										EntityFactory(RenderSystem* rs, PhysicsSystem* ps, ControlSystem* ctls, CameraSystem* cs, GunSystem* gs, FiringSystem* fs, std::map<TextureID, SDL_Texture*>* th);
										~EntityFactory();
	
	Entity*								CreateEntity(Entity::Type t);

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

private:
	std::map<TextureID, SDL_Texture*>*	_textureHolder;

	RenderSystem*						_renderSystem;
	CameraSystem*						_cameraSystem;
	PhysicsSystem*						_physicSystem;
	ControlSystem*						_controlSystem;
	GunSystem*							_gunSystem;
	FiringSystem*						_firingSystem;
};
#endif