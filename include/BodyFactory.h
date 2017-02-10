#ifndef BODYFACTORY_H
#define BODYFACTORY_H
#include "Box2D\Box2D.h"

class BodyFactory
{
	b2World*		_b2world;
public:
	BodyFactory(b2World* b2world);
	~BodyFactory();

	b2Body*			CreateBoxBody(b2BodyType type, b2Vec2 pos, float rotation, b2Vec2 size,  bool isSensor);
	b2Body*			CreatePolyBody(b2BodyType type, b2Vec2 pos, float rotation, b2Vec2* vertices, int count, bool isSensor);



private:
	

};
#endif