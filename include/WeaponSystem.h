#pragma once

#include "InteractionSystem.h"
#include "Camera2D.h"
#include "InteractionSystemEvents.h"

class WeaponSystem : public InteractionSystem
{
public:
										WeaponSystem(std::map<InteractionSystemEvent, std::vector<std::pair<Entity*, Entity*>>>& events, float updateRate = 0.f);
										~WeaponSystem();

	void								Initialize(Camera2D::Camera* camera);

	virtual void						RemoveEntity(Entity* gameObject, bool firstObject) override;

	void								Process(float dt = 0.f) override;
	void								ListenForEvents();
	void								WeaponCreationEvent();
	void								WeaponBulletAddition();

private:
	Camera2D::Camera*					_camera;
	std::map<InteractionSystemEvent,
		std::vector<
		std::pair<Entity*, Entity*>>>&  _interactionSystemEvents;

	const InteractionSystemEvent		WEAPON_CREATED;
	const InteractionSystemEvent		BULLET_ADDITION;
};

