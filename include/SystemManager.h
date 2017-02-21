#pragma once

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "GunSystem.h"
#include "AISystem.h"
#include "DestructionSystem.h"
#include "UISystem.h"
#include "WeaponSystem.h"
#include "WaypointSystem.h"

class SystemManager
{
public:
	enum class SystemType
	{
		UI,
		Render,
		Destruction,
		Physics,
		Camera,
		Collision,
		Gun,
		AI,
		World,
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

	void								Initialize(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, Graph* waypoints, int width, int height);
	void								InitializeSystems(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, Graph* waypoints, int width, int height);
	void								InitializeInteractionSystems();

	void								Process(float dt = 0.f);
	void								TryToDestroy(SystemMapIterator& it, float dt);
	void								ProcessAllSystems(SystemMapIterator& it, float dt);
	void								ProcessAllInteractionSystems(SystemMapIterator& it, float dt);
	void								DestroyBasedOnType(Entity*& entity);
	void								Render(float dt = 0.f);

	void								AddEntity(SystemType type, Entity* entity);
	void								AddEntity(InteractionSystemType type, Entity* actor, Entity* item);

	RenderSystem*						GetRenderSystem();
	PhysicsSystem*						GetPhysicsSystem();
	CameraSystem*						GetCameraSystem();
	CollisionSystem*					GetCollisionSystem();
	GunSystem*							GetGunSystem();
	UISystem*							GetUISystem();
	AISystem*							GetAISystem();

	WeaponSystem*						GetWeaponInteractionSystem();

	Camera2D::Camera&					GetCamera();

private:
	std::map<InteractionSystemType, 
		InteractionSystem*>				_interactionSystems;
	std::map<SystemType, System*>		_systems;
};

