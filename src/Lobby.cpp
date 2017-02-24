#include "Lobby.h"

#include <string>
#include <sstream>

Lobby::Lobby()
	: _cameraSystem(CAMERA_SYSTEM_UPDATE)
	, _renderSystem()
	, _functionMaster()
	, _uiSystem(0)
{
	_renderSystem.Initialize(_renderer, &_cameraSystem.getCamera());
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Lobby::~Lobby()
{
}

void Lobby::Initialize(SDL_Renderer* renderer)
{
	_renderer = renderer;
	_running = true;
	_swapScene = CurrentScene::LOBBY;
	_selectedItemIndex = 0;

	_cameraSystem.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
	_uiSystem.Initialize(_renderer);

	Entity* ui = new Entity(EntityType::UI);
	_uiSystem.AddEntity(ui);

	LoadContent();


	Refresh();
}

int Lobby::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update
	
	//UPDATE HERE	
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();
	//save the curent time for next frame


	NetworkHandler& network = NetworkHandler::Instance();

	ReceivedData receivedData = network.Receive();

	if (receivedData.Empty() == false)
	{

		switch (receivedData.GetType())
		{
		case MessageType::Connect:
		{
			ConnectData data = receivedData.GetData<ConnectData>();
			std::cout << "Got assigned ID: " << data.id << std::endl;
			network.SetPlayerID(data.id);
			network.SetConnected(true);
			break;
		}
		case MessageType::SessionList:
		{
			SessionListData data = receivedData.GetData<SessionListData>();
			std::cout << "Sessions Available: " << data.count << std::endl;
			std::vector<Session> sessions;
			for (int i = 0; i < data.count; i++)
			{
				Session s;
				s.id = data.sessionIDs[i];
				s.currentPlayers = data.currentPlayers[i];
				std::cout << "Session ID: " << s.id;
				std::cout << "  Players: " << s.currentPlayers << "/" << data.maxPlayers << std::endl;
				sessions.push_back(s);
			}
			Refresh(sessions, data.maxPlayers);
			break;
		}//two below are sent at the same time in response to client sending joinSession. 
		case MessageType::JoinSession:
		{
			JoinSessionData data = receivedData.GetData<JoinSessionData>(); //client gets session id, switch scene to player list, get id before or after scene switch??
			std::cout << "Joined session: " << data.sessionID << std::endl;
			network.SetSessionID(data.sessionID);
			break;
		}
		case MessageType::PlayerList:
		{
			std::cout << "Player list data in lobby received" << std::endl;
			PlayerListData data = receivedData.GetData<PlayerListData>();
			for (int i = 0; i < data.count; i++)
			{
				std::cout << "Player: " << data.players[i] << std::endl;
			}
			Refresh(data.players);
			break;
		}
		//have to add in host change here
		}
	}



	_lastTime = currentTime;
	return (int)_swapScene;
}

void Lobby::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
	_renderSystem.Process();
	_uiSystem.Process();

	SDL_RenderPresent(_renderer);
}

bool Lobby::IsRunning()
{
	if (_swapScene != CurrentScene::LOBBY) { _swapScene = CurrentScene::LOBBY; }
	return _running;
}

void Lobby::Start()
{
	//Input
	BindInput();

	ConnectData data;
	NetworkHandler::Instance().Send(&data);
}

void Lobby::Stop()
{
	_inputManager->ResetKey(Event::BACKSPACE);
	_inputManager->ResetKey(Event::h);
}

void Lobby::OnEvent(EventListener::Event evt)
{
	if (_running)
	{
		switch (evt)
		{
		case Event::ESCAPE:
			_running = false;
		}
	}
}

void Lobby::LoadContent()
{
	_uiSystem.CreateDisplayText("Sessions", SCREEN_WIDTH / 2, 50);
	_uiSystem.CreateDisplayText("________", SCREEN_WIDTH / 2, 60);
}

void Lobby::CleanUp()
{
	//DESTROY HERE

	//SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

void Lobby::BindInput()
{
	Command* enterIn = new InputCommand([&]()
	{
		if (_selectedItemIndex == _uiSystem._interactiveTextRectangle.size() - 1) 
		{
			//there should be no quit button in here
			//_running = false;
		}
		else 
		{
			_swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1);
		}
	}, Type::Press);

	_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* pIn = new InputCommand([&]()
	{
		_uiSystem.DeleteText();
	}, Type::Press);

	_inputManager->AddKey(Event::p, pIn, this);

	Command* oIn = new InputCommand([&]()
	{
		Start();
	}, Type::Press);

	_inputManager->AddKey(Event::o, oIn, this);


	Command* hIn = new InputCommand([&]()
	{
		JoinSessionData data;
		NetworkHandler::Instance().Send(&data);
		//switch the player list scene
		std::cout << "Sending player list data." << std::endl;
		//change scene to players

	}, Type::Press);

	_inputManager->AddKey(Event::h, hIn, this);


	Command* backIn = new InputCommand([&]() { 
		NetworkHandler::Instance().Disconnect();
		_swapScene = Scene::CurrentScene::MAIN_MENU; 
	}, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	_inputManager->AddListener(Event::ESCAPE, this);
}

void Lobby::MoveUp()
{
	if (_selectedItemIndex - 1 >= 0)
	{
		_selectedItemIndex--;
	}
	// Jump to bottom
	else
	{
		// _textRectangle.size() - 2 = 1 before icon
		_selectedItemIndex = _uiSystem._interactiveTextRectangle.size() - 1;
	}
	_uiSystem._displayTextRectangle.back().x = _uiSystem._interactiveTextRectangle[_selectedItemIndex].x + _uiSystem._interactiveTextRectangle[_selectedItemIndex].w + 50;
	_uiSystem._displayTextRectangle.back().y = _uiSystem._interactiveTextRectangle[_selectedItemIndex].y;
}

void Lobby::MoveDown()
{
	if (_selectedItemIndex < _uiSystem._interactiveTextRectangle.size() - 1)
	{
		_selectedItemIndex++;
	}
	// Jump to top
	else
	{
		_selectedItemIndex = 0;
	}
	_uiSystem._displayTextRectangle.back().x = _uiSystem._interactiveTextRectangle[_selectedItemIndex].x + _uiSystem._interactiveTextRectangle[_selectedItemIndex].w + 50;
	_uiSystem._displayTextRectangle.back().y = _uiSystem._interactiveTextRectangle[_selectedItemIndex].y;
}

int Lobby::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}

void Lobby::Refresh(const std::vector<Session>& sessions, int maxPlayers)
{
	_uiSystem.DeleteText();
	if (sessions.empty())
	{
		_uiSystem.CreateText("No sessions available. Create a new one or refresh.", 50, 200);
	}
	for (int i = 0; i < sessions.size(); i++)
	{
		std::ostringstream oss;
		oss << "[" << sessions[i].id << "]" << " - " << sessions[i].currentPlayers << "/" << maxPlayers;
		std::string var = oss.str();

		if (i == 0)
		{
			_uiSystem.CreateText(var, 50, 200);
		}

		else
		{
			_uiSystem.CreateText(var, 50, _uiSystem._interactiveTextRectangle[i - 1].y + 50);
		}
	}
}

void Lobby::Refresh(const std::vector<int>& players)
{
	_uiSystem.DeleteText();
	for (int i = 0; i < players.size(); i++)
	{
		std::ostringstream oss;
		oss << "Player [" << players[i] << "]";
		if (players[i] == NetworkHandler::Instance().GetPlayerID())
		{
			oss << " (you!).";
		}
		oss << std::endl;

		std::string var = oss.str();
		if (i == 0)
		{
			_uiSystem.CreateText(var, 50, 200);
		}

		else
		{
			_uiSystem.CreateText(var, 50, _uiSystem._interactiveTextRectangle[i - 1].y + 50);
		}
	}
}
