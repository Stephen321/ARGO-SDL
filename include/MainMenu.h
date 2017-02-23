#pragma once

#include "Scene.h"

class MainMenu : public Scene
{
public:
	MainMenu();
	~MainMenu();

	void							Initialize(SDL_Renderer* renderer) override;

	int								Update() override;
	void							Render() override;

	bool							IsRunning() override;

	void							Start() override;
	void							Stop() override;

	void							OnEvent(Event evt) override;

private:
	void							BindInput() override;

	void							LoadContent() override;
	void							CleanUp() override;

private:
	LevelLoader						_levelLoader;

	RenderSystem					_renderSystem;
	FunctionMaster					_functionMaster;
	CameraSystem					_cameraSystem;
	UISystem						_uiSystem;

	CurrentScene					_swapScene;

private:
	void							MoveUp();
	void							MoveDown();

	int								GetPressedItem();
	int								_selectedItemIndex;
};

