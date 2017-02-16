#include "SystemManager.h"

#include "ConstHolder.h"


SystemManager::SystemManager()
{
}


SystemManager::~SystemManager()
{
	

}

void SystemManager::Initialize(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, Graph* map, int width, int height)
{
 	InitializeSystems(renderer, entities, entityFactory, bodyFactory, world, map, width, height);
	InitializeInteractionSystems();
}

#pragma region Initialization

void SystemManager::InitializeSystems(SDL_Renderer*& renderer, std::vector<Entity*>* entities, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, Graph* map, int width, int height)
{
	//SETUP CAMERA SYSTEM
	CameraSystem* cameraSystem = new CameraSystem(CAMERA_SYSTEM_UPDATE);
	cameraSystem->Initialize(width, height);
	_systems[SystemType::Camera] = cameraSystem;

	//SETUP RENDER SYSTEM
	RenderSystem* rendererSystem = new RenderSystem(0);
	rendererSystem->Initialize(renderer, &cameraSystem->getCamera());
	_systems[SystemType::Render] = rendererSystem;

	//SETUP PHYSICS SYSTEM
	PhysicsSystem* physicsSystem = new PhysicsSystem(0);
	_systems[SystemType::Physics] = physicsSystem;

	//SETUP CONTROL SYSTEM
	ControlSystem* controlSystem = new ControlSystem(0);
	controlSystem->Initialize(&cameraSystem->getCamera());
	_systems[SystemType::Control] = controlSystem;

	//SETUP COLLISION SYSTEM
	CollisionSystem* collisionSystem = new CollisionSystem(COLLISION_SYSTEM_UPDATE);
	world->SetContactListener(collisionSystem);
	_systems[SystemType::Collision] = collisionSystem;

	//SETUP GUN SYSTEM
	GunSystem* gunSystem = new GunSystem(0);
	gunSystem->Initialize(entities, entityFactory, bodyFactory);
	_systems[SystemType::Gun] = gunSystem;

	//SETUP AI SYSTEM
	AISystem* aiSystem = new AISystem(0);
	aiSystem->Initialize(map);
	_systems[SystemType::AI] = aiSystem;
}
void SystemManager::InitializeInteractionSystems()
{
	//SETUP WEAPON INTERACTION SYSTEM
	WeaponSystem* weaponSystem = new WeaponSystem(0);
	_interactionSystems[InteractionSystemType::Weapon] = weaponSystem;
}

#pragma endregion


void SystemManager::Process(float dt)
{
	//Skip RenderSystem
	SystemMapIterator it = _systems.begin();
	it++;

	for (; it != _systems.end(); ++it)
	{
		it->second->Process(dt);
	}

	for (InteractionSystemMapIterator it = _interactionSystems.begin(); it != _interactionSystems.end(); ++it)
	{
		it->second->Process(dt);
	}
}

void SystemManager::Render(float dt)
{
	_systems[SystemType::Render]->Process();
}


void SystemManager::AddEntity(SystemType type, Entity* entity)
{
	_systems[type]->AddEntity(entity);
}
void SystemManager::AddEntity(InteractionSystemType type, Entity* actor, Entity* item)
{
	_interactionSystems[type]->AddEntity(actor, item);
}

#pragma region Get Systems

RenderSystem* SystemManager::GetRenderSystem()
{
	RenderSystem* renderSystem = static_cast<RenderSystem*>(_systems[SystemType::Render]);
	return renderSystem;
}
PhysicsSystem* SystemManager::GetPhysicsSystem()
{
	PhysicsSystem* physicsSystem = static_cast<PhysicsSystem*>(_systems[SystemType::Physics]);
	return physicsSystem;
}
ControlSystem* SystemManager::GetControlSystem()
{
	ControlSystem* controlSystem = static_cast<ControlSystem*>(_systems[SystemType::Control]);
	return controlSystem;
}
CameraSystem* SystemManager::GetCameraSystem()
{
	CameraSystem* cameraSystem = static_cast<CameraSystem*>(_systems[SystemType::Camera]);
	return cameraSystem;
}
CollisionSystem* SystemManager::GetCollisionSystem()
{
	CollisionSystem* collisionSystem = static_cast<CollisionSystem*>(_systems[SystemType::Collision]);
	return collisionSystem;
}
GunSystem* SystemManager::GetGunSystem()
{
	GunSystem* gunSystem = static_cast<GunSystem*>(_systems[SystemType::Gun]);
	return gunSystem;
}
AISystem* SystemManager::GetAISystem()
{
	AISystem* aiSystem = static_cast<AISystem*>(_systems[SystemType::AI]);
	return aiSystem;
}

#pragma endregion

#pragma region Get Interaction Systems

WeaponSystem* SystemManager::GetWeaponInteractionSystem()
{
	WeaponSystem* weaponInteractionSystemType = static_cast<WeaponSystem*>(_interactionSystems[InteractionSystemType::Weapon]);
	return weaponInteractionSystemType;
}

#pragma endregion



Camera2D::Camera& SystemManager::GetCamera()
{
	CameraSystem* cameraSystem = static_cast<CameraSystem*>(_systems[SystemType::Camera]);
	return cameraSystem->getCamera();
}