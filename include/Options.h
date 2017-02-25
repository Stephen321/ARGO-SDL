#pragma once

#include <SDL.h>

#include "SDL_image.h"
#include "SDL_ttf.h"

#include "Debug.h"

#include "Camera2D.h"
#include "FLInputManager.h"
#include "Scene.h"

#include "CameraSystem.h"

#include <vector>
#include <queue>
#include <map>

#include "ResourceIdentifier.h"
#include "FunctionMaster.h"
#include "RenderSystem.h"

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
};

