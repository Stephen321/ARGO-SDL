#pragma once

class Scene
{
public:
	Scene();
	~Scene();

	virtual int Update();
	virtual void Render();
	virtual bool IsRunning();

	enum CurrentScene 
	{
		MAIN_MENU,
		GAME,
	};
};