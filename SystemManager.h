#pragma once

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "ControlSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "GunSystem.h"
#include "FiringSystem.h"

#include "WeaponSystem.h"



class SystemManager
{
public:
	enum class SystemType
	{
		Render,
		Physics,
		Control,
		Camera,
		Collision,
		Gun,
		Firing,
	};

	enum class InteractionSystemType
	{
		Weapon,
	};

public:
	typedef std::map<InteractionSystemType, InteractionSystem*>::iterator InteractionSystemMapIterator;
	typedef std::map<SystemType, System*>::iterator SystemMapIterator;

										SystemManager();
										~SystemManager();

	void								Initialize(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height);
	void								InitializeSystems(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, int width, int height);
	void								InitializeInteractionSystems();

	void								Process(float dt = 0.f);
	void								Render(float dt = 0.f);

	void								AddEntity(SystemType type, Entity* entity);
	void								AddEntity(InteractionSystemType type, Entity* actor, Entity* item);

	RenderSystem*						GetRenderSystem();
	PhysicsSystem*						GetPhysicsSystem();
	ControlSystem*						GetControlSystem();
	CameraSystem*						GetCameraSystem();
	CollisionSystem*					GetCollisionSystem();
	GunSystem*							GetGunSystem();
	FiringSystem*						GetFiringSystem();

	WeaponSystem*						GetWeaponInteractionSystem();

	Camera2D::Camera&					GetCamera();

private:
	std::map<InteractionSystemType, 
		InteractionSystem*>				_interactionSystems;
	std::map<SystemType, System*>		_systems;
};

