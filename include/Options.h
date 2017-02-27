#pragma once

#include "Scene.h"

class Options : public EventListener, public Scene
{
public:
	Options();
	~Options();

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
	RenderSystem					_renderSystem;
	FunctionMaster					_functionMaster;
	CameraSystem					_cameraSystem;
	UISystem						_uiSystem;

private:
	void							MoveUp();
	void							MoveDown();

	int								GetPressedItem();
	int								_selectedItemIndex;


	std::string						GetMusicVolume();
	std::string						GetHumVolume();
};

