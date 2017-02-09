#ifndef BODYFACTORY_H
#define BODYFACTORY_H
#include "Box2D\Box2D.h"

class BodyFactory
{
	b2World*			_b2world;
public:
	BodyFactory(b2World *b2world);
	~BodyFactory();

	b2Body*			CreateCharacterBody();
	b2Body*			CreateCollectibleBody();
	b2Body*			CreateObstacleBody();
	b2Body*			CreateProjectileBody();

private:
	

};
#endif