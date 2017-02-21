#pragma once

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "GunSystem.h"
#include "DestructionSystem.h"

#include "WeaponSystem.h"
#include "FlagCheckpointSystem.h"



class SystemManager
{
public:
	enum class SystemType
	{
		Render,
		Destruction,
		Physics,
		Camera,
		Collision,
		Gun,
	};

	enum class InteractionSystemType
	{
		Weapon,
		Flag,
	};

public:
	typedef std::map<InteractionSystemType, InteractionSystem*>::iterator InteractionSystemMapIterator;
	typedef std::map<SystemType, System*>::iterator SystemMapIterator;

										SystemManager();
										~SystemManager();

	void								Initialize(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height);
	void								InitializeSystems(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height);
	void								InitializeInteractionSystems();
	void								PostInitialize(std::vector<Entity*>& checkpoints);

	void								Process(float dt = 0.f);
	void								TryToDestroy(SystemMapIterator& it, float dt);
	void								ProcessAllSystems(SystemMapIterator& it, float dt);
	void								ProcessAllInteractionSystems(SystemMapIterator& it, float dt);
	void								DestroyBasedOnType(Entity*& entity);
	void								Render(float dt = 0.f);

	void								AddEntity(SystemType type, Entity* entity);
	void								AddEntity(InteractionSystemType type, Entity* actor, Entity* item);

	void								RemoveEntity(SystemType type, Entity* entity);
	void								RemoveEntity(InteractionSystemType type, Entity* actor, Entity* item);
	void								RemoveEntity(InteractionSystemType type, Entity* actor, bool firstItem);

	RenderSystem*						GetRenderSystem();
	PhysicsSystem*						GetPhysicsSystem();
	CameraSystem*						GetCameraSystem();
	CollisionSystem*					GetCollisionSystem();
	GunSystem*							GetGunSystem();

	WeaponSystem*						GetWeaponInteractionSystem();
	FlagCheckpointSystem*				GetFlagCheckpointSystem();

	Camera2D::Camera&					GetCamera();

private:
	std::map<InteractionSystemType, 
		InteractionSystem*>				_interactionSystems;
	std::map<SystemType, System*>		_systems;
};

