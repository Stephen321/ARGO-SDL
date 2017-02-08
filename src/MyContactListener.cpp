#include "MyContactListener.h"


//possible collisions
void MyContactListener::BeginContact(b2Contact* contact) 
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	
}
void MyContactListener::EndContact(b2Contact* contact) 
{
	void* bodyAUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyBUserData = contact->GetFixtureB()->GetBody()->GetUserData();

	
}
