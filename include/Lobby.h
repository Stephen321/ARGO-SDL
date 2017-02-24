#pragma once

#include "Scene.h"
#include "NetworkHandler.h"

class Lobby : public Scene
{
public:
	Lobby();
	~Lobby();

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
	struct Session {
		int id;
		int currentPlayers;
	};

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

	void							Refresh(const std::vector<Session>& sessions = std::vector<Session>(), int maxPlayers = 0);
	void							Refresh(const std::vector<int>& players);
};

