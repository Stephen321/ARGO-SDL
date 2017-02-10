#pragma once

#include "Component.h"

#include <vector>

class Entity
{
public:
	enum class Type
	{ //draw order is determined by the below order
		Checkpoint,
		Wall,
		Tile,
		Player,
		Flag,
		Weapon,
	};

public:
								Entity(Type type);
								~Entity();

	void						AddComponent(Component* component);
	void						RemoveComponent(Component::Type componentType);

	Component*					GetComponent(Component::Type componentType);
	std::vector<Component*>&	GetComponents();
	Type						GetType() const;

private:
	std::vector<Component*>		_components;
	Type						_type;
};

