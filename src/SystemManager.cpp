#include "SystemManager.h"

#include "ConstHolder.h"


SystemManager::SystemManager()
	: _creationRequests(std::vector<std::pair<EntityType, std::vector<float>>>())
{
}


SystemManager::~SystemManager()
{
	delete _creationSystem;

	SystemMapIterator it = _systems.begin();
	for (; it != _systems.end(); ++it)
	{
		delete it->second;
	}

	for (InteractionSystemMapIterator it = _interactionSystems.begin(); it != _interactionSystems.end(); ++it)
	{
		delete it->second;
	}
}

void SystemManager::Initialize(SDL_Renderer*& renderer, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, Graph* waypoints, int width, int height)
{
 	InitializeSystems(renderer, entityFactory, bodyFactory, world, waypoints, width, height);
	InitializeInteractionSystems();
}

#pragma region Initialization

void SystemManager::InitializeSystems(SDL_Renderer*& renderer, EntityFactory* entityFactory, BodyFactory* bodyFactory, b2World* world, Graph* waypoints, int width, int height)
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

	//SETUP COLLISION SYSTEM
	CollisionSystem* collisionSystem = new CollisionSystem(_interactionSystemEvents, COLLISION_SYSTEM_UPDATE);
	world->SetContactListener(collisionSystem);
	_systems[SystemType::Collision] = collisionSystem;

	//SETUP UI SYSTEM
	UISystem* uiSystem = new UISystem(0);
	uiSystem->Initialize(renderer, &cameraSystem->getCamera());
	_systems[SystemType::UI] = uiSystem;

	//SETUP GUN SYSTEM
	GunSystem* gunSystem = new GunSystem(_creationRequests, 0);
	_systems[SystemType::Gun] = gunSystem;

	//SETUP AI SYSTEM
	AISystem* aiSystem = new AISystem(0);
	aiSystem->Initialize(waypoints);
	_systems[SystemType::AI] = aiSystem;

	//SETUP WORLD SYSTEM
	WaypointSystem* waypointSystem = new WaypointSystem(0);
	waypointSystem->Initialize(waypoints);
	_systems[SystemType::World] = waypointSystem;

	//SETUP Destroy SYSTEM
	DestructionSystem* destructionSystem = new DestructionSystem(0);
	_systems[SystemType::Destruction] = destructionSystem;

	//SETUP Destroy SYSTEM
	_creationSystem = new CreationSystem(_creationRequests);
	_creationSystem->Initialize(entityFactory, bodyFactory);
}
void SystemManager::InitializeInteractionSystems()
{
	//SETUP WEAPON INTERACTION SYSTEM
	WeaponSystem* weaponSystem = new WeaponSystem(_interactionSystemEvents, 0);
	weaponSystem->Initialize(&GetCamera());
	_interactionSystems[InteractionSystemType::Weapon] = weaponSystem;

	FlagCheckpointSystem* flagSystem = new FlagCheckpointSystem(_interactionSystemEvents, 0);
	_interactionSystems[InteractionSystemType::Flag] = flagSystem;
}

#pragma endregion

void SystemManager::PostInitialize(Entity*& player)
{
	_creationSystem->Process(0);

	Entity* flag = nullptr;

	std::vector<Entity*> checkpoints = std::vector<Entity*>();

	while (!_creationSystem->Empty())
	{
		std::pair<std::vector<SystemType>, Entity*>& systemCreatedEntity = _creationSystem->GetSystemCreatedEntity();

		for (int i = 0; i < systemCreatedEntity.first.size(); i++)
		{
			AddEntity(systemCreatedEntity.first.at(i), systemCreatedEntity.second);
		}

		if (systemCreatedEntity.second->GetType() == EntityType::Player)
		{
			player = systemCreatedEntity.second;
		}
		else if (systemCreatedEntity.second->GetType() == EntityType::Flag)
		{
			flag = systemCreatedEntity.second;
		}
		else if (systemCreatedEntity.second->GetType() == EntityType::Checkpoint)
		{
			checkpoints.push_back(systemCreatedEntity.second);
		}

		_creationSystem->EntityToSystemAssigned();
	}


	//SETUP FLAG INTERACTION SYSTEM
	GetFlagCheckpointSystem()->Initialize(checkpoints);
}

void SystemManager::Process(float dt)
{
	//Skip RenderSystem and UI
	SystemMapIterator it = _systems.begin();
	it++;
	it++;

	TryToDestroy(it, dt);

	TryToCreateEntities(dt);

	ProcessAllSystems(it, dt);

	ProcessAllInteractionSystems(it, dt);
}

void SystemManager::TryToCreateEntities(float dt)
{
	_creationSystem->Process(dt);

	while (!_creationSystem->Empty())
	{
		std::pair<std::vector<SystemType>, Entity*>& systemCreatedEntity = _creationSystem->GetSystemCreatedEntity();

		for (int i = 0; i < systemCreatedEntity.first.size(); i++)
		{
			AddEntity(systemCreatedEntity.first.at(i), systemCreatedEntity.second);

		}

		_creationSystem->EntityToSystemAssigned();
	}
}

void SystemManager::TryToDestroy(SystemMapIterator& it, float dt)
{
	DestructionSystem* destructionSystem = static_cast<DestructionSystem*>(it->second);
	destructionSystem->Process(dt);

	int index = destructionSystem->GetEntitiesToBeDestroyed().size() - 1;
	while (index >= 0)
	{
		DestroyBasedOnType(destructionSystem->GetEntitiesToBeDestroyed().at(index));
		destructionSystem->DestroyEntity();

		index--;
	}
}

void SystemManager::ProcessAllSystems(SystemMapIterator& it, float dt)
{
	for (; it != _systems.end(); ++it)
	{
		it->second->Process(dt);
	}
}
void SystemManager::ProcessAllInteractionSystems(SystemMapIterator& it, float dt)
{
	for (InteractionSystemMapIterator it = _interactionSystems.begin(); it != _interactionSystems.end(); ++it)
	{
		it->second->Process(dt);
	}
}

void SystemManager::DestroyBasedOnType(Entity*& entity)
{
	switch (entity->GetType())
	{
	case EntityType::Flag:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::Checkpoint:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::Tile:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::Obstacle:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		break;
	case EntityType::Bullet:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		break;
	case EntityType::PowerUp:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		//Implement Later
		break;
	case EntityType::AI:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		break;
	case EntityType::Player:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Physics]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Camera]->RemoveEntity(entity->GetType(), entity);
		break;
	case EntityType::Point:
		//Implement Later??
		break;
	case EntityType::Weapon:
		_systems[SystemType::Render]->RemoveEntity(entity->GetType(), entity);
		_systems[SystemType::Gun]->RemoveEntity(entity->GetType(), entity);
		//fix weapon deletion
		static_cast<WeaponSystem*>(_interactionSystems[InteractionSystemType::Weapon])->RemoveEntity(entity, false);
		break;

	default:
		break;
	}
}

void SystemManager::Render(float dt)
{
	_systems[SystemType::Render]->Process(dt);
	_systems[SystemType::UI]->Process(dt);
}

void SystemManager::AddRequest(std::pair<EntityType, std::vector<float>>& creationRequest)
{
	_creationRequests.push_back(creationRequest);
}
void SystemManager::AddRequest(std::vector<std::pair<EntityType, std::vector<float>>>& creationRequests)
{
	for (int i = 0; i < creationRequests.size(); i++)
	{
		_creationRequests.push_back(creationRequests.at(i));
	}
}

void SystemManager::AddEntity(SystemType type, Entity* entity)
{
	_systems[type]->AddEntity(entity);
}
void SystemManager::AddEntity(InteractionSystemType type, Entity* actor, Entity* item)
{
	_interactionSystems[type]->AddEntity(actor, item);
}

void SystemManager::RemoveEntity(SystemType type, Entity* entity)
{
	_systems[type]->RemoveEntity(entity->GetType(), entity);
}
void SystemManager::RemoveEntity(InteractionSystemType type, Entity* actor, Entity* item)
{
	_interactionSystems[type]->RemoveEntity(actor, item);
}
void SystemManager::RemoveEntity(InteractionSystemType type, Entity* entity, bool firstItem)
{
	_interactionSystems[type]->RemoveEntity(entity, firstItem);
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

UISystem * SystemManager::GetUISystem()
{
	UISystem* uiSystem = static_cast<UISystem*>(_systems[SystemType::UI]);
	return uiSystem;
}

#pragma endregion

#pragma region Get Interaction Systems

WeaponSystem* SystemManager::GetWeaponInteractionSystem()
{
	WeaponSystem* weaponInteractionSystemType = static_cast<WeaponSystem*>(_interactionSystems[InteractionSystemType::Weapon]);
	return weaponInteractionSystemType;
}

FlagCheckpointSystem* SystemManager::GetFlagCheckpointSystem()
{
	FlagCheckpointSystem* flagInteractionSystemType = static_cast<FlagCheckpointSystem*>(_interactionSystems[InteractionSystemType::Flag]);
	return flagInteractionSystemType;
}

#pragma endregion



Camera2D::Camera& SystemManager::GetCamera()
{
	CameraSystem* cameraSystem = static_cast<CameraSystem*>(_systems[SystemType::Camera]);
	return cameraSystem->getCamera();
}