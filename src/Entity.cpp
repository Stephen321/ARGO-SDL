#include "Entity.h"



Entity::Entity(Type type)
	: _type(type)
{
}


Entity::~Entity()
{
	for (int i = 0; i < _components.size(); i++)
	{
		delete _components[i];
		break;
	}
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
			delete _components[i];
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
	return nullptr;
}
std::vector<Component*>& Entity::GetComponents()
{
	return _components;
}

Entity::Type Entity::GetType() const
{
	return _type;
}
