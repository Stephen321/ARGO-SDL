#include "BodyFactory.h"
BodyFactory::BodyFactory(b2World* b2world) : _b2world(b2world)
{

}

BodyFactory::~BodyFactory()
{

}

b2Body* BodyFactory::CreateBoxBody(b2BodyType type, b2Vec2 pos, float rotation, b2Vec2 size, bool isSensor)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position = pos;
	bodyDef.angle = 0;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(size.x, size.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.isSensor = isSensor;
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* BodyFactory::CreatePolyBody(b2BodyType type, b2Vec2 pos, float rotation, b2Vec2* vertices, int count, bool isSensor)
{
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position = pos;
	bodyDef.angle = rotation;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.Set(vertices, count);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;
	fixtureDef.isSensor = isSensor;
	body->CreateFixture(&fixtureDef);

	return body;
}