#pragma once
#include "System.h"
#include "Box2D\Box2D.h"


class CollisionSystem : public System, public b2ContactListener
{
public:
				CollisionSystem(float updateRate = 0.f);
				~CollisionSystem();

	void		Process(float dt = 0.f) override;

	void		BeginContact(b2Contact* contact) override;
	void		EndContact(b2Contact* contact) override;

	void		PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	void		PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

private:
	void		FindPlayer(b2Contact* contact, Entity*& player, Entity*& other);
};
