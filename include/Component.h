#pragma once



class Component
{
public:
	enum class Type
	{
		Health,
		Bounds,
		Physics,
		Collider,
		Sprite,
		Control,
		AI,
	};

public:
	Component(Type type)
		: _type(type)
	{
	}

	~Component()
	{
	}


	Type GetType() const
	{
		return _type;
	}

private:
	Type	_type;
};

