#include "Entity.h"



Entity::Entity(Type type)
	: _type(type)
{
}


Entity::~Entity()
{
}


void Entity::AddComponent(Component* component)
{
	_components.push_back(component);
}
void Entity::RemoveComponent(Component::Type componentType)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->GetType() == componentType)
		{
			_components.erase(_components.begin() + i);
			break;
		}
	}
}

Component* Entity::GetComponent(Component::Type componentType)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->GetType() == componentType)
		{
			return _components.at(i);
		}
	}
}
std::vector<Component*>& Entity::GetComponents()
{
	return _components;
}

Entity::Type Entity::GetType() const
{
	return _type;
}
