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

class EntityFactory
{
public: 
	EntityFactory(RenderSystem* rs, PhysicsSystem* ps, ControlSystem* ctls, CameraSystem* cs, std::map<TextureID, SDL_Texture*>* th);
	~EntityFactory();
	
	Entity* CreateEntity(Entity::Type t);

private:
	std::map<TextureID, SDL_Texture*>*	_textureHolder;
	RenderSystem*						_renderSystem;
	CameraSystem*						_cameraSystem;
	PhysicsSystem*						_physicSystem;
	ControlSystem*						_controlSystem;

	Entity*			CreatePlayerEntity();
	Entity*			CreateAIEntity();
	Entity*			CreateObstacleEntity();
	Entity*			CreatePowerUpEntity();
	Entity*			CreateBulletEntity();
	Entity*			CreateCheckpointEntity();
	Entity*			CreateFlagEntity();
	Entity*			CreateTileEntity();
	Entity*			CreatePointEntity();
};
#endif