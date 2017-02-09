#include "BodyFactory.h"
BodyFactory::BodyFactory(b2World* b2world) : _b2world(b2world)
{

}

BodyFactory::~BodyFactory()
{

}

b2Body* BodyFactory::CreateCharacterBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0, 0);
	bodyDef.angle = 0;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	return body;
}

b2Body* BodyFactory::CreateCollectibleBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0, 0);
	bodyDef.angle = 0;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	/*
	b2PolygonShape boxShape;
	boxShape.SetAsBox(200, 50);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;

	body->CreateFixture(&fixtureDef);*/
	return body;
}

b2Body* BodyFactory::CreateObstacleBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody; 
	bodyDef.position.Set(0, 0); 
	bodyDef.angle =0; 
	b2Body* body = _b2world->CreateBody(&bodyDef);

	/*
	b2PolygonShape boxShape;
	boxShape.SetAsBox(200, 50);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;

	body->CreateFixture(&fixtureDef);*/
	return body;
}

b2Body* BodyFactory::CreateProjectileBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0, 0);
	bodyDef.angle = 0;
	b2Body* body = _b2world->CreateBody(&bodyDef);

	/*
	b2PolygonShape boxShape;
	boxShape.SetAsBox(200, 50);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1;

	body->CreateFixture(&fixtureDef);*/
	return body;
}
