#pragma once

#include "Scene.h"
#include "RenderSystem.h"
#include "CameraSystem.h"

class About : public EventListener, public Scene
{
public:
	About();
	~About();

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
	CameraSystem					_cameraSystem;
};

