#include "CollisionSystem.h"


CollisionSystem::CollisionSystem(float updateRate)
	: System(updateRate)
{
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Process(float dt)
{
	System::Process(dt);
	if (_canUpdate)
	{
		_canUpdate = false;
		//for (EntityMapIterator it = _entities.begin(); it != _entities.end(); ++it)
		//{
		//	for (Entity* e : (*it).second)
		//	{

		//	}
		//}
	}
}

void CollisionSystem::BeginContact(b2Contact* contact)
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();


}
void CollisionSystem::EndContact(b2Contact* contact)
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();
}
