#include "Lobby.h"

#include "ConstHolder.h"

#include <string>
#include <sstream>

Lobby::Lobby()
	: _uiSystem(0.f)
	, _startingGame(false)
	, _connectTimer(0.f)
	, _readyTimer(READY_COUNTDOWN)
	, _startReadyTimer(false)
	, _countdownTextId(-1)
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Lobby::~Lobby()
{
}

void Lobby::Initialize(SDL_Renderer* renderer, std::vector<int>* ids)
{
	Scene::Initialize(renderer);

	_ids = ids;

	_running = true;
	_selectedItemIndex = 0;

	// UI
	_uiSystem.Initialize(_renderer);

	Entity* ui = new Entity(EntityType::UI);
	_uiSystem.AddEntity(ui);

	Start();// - activate when loaded
	LoadContent();


	Refresh();

	_lastTime = LTimer::gameTime();
	_readyTimer = READY_COUNTDOWN;
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

	if (network.GetConnected() == false)
	{
		_connectTimer += dt;
		if (_connectTimer > CONNECT_RETRY)
		{
			ConnectData data;
			network.Send(&data);
			_connectTimer = 0.f;
		}
	}


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
			network.SetHost(data.host);
			std::cout << "host :  " << data.host << std::endl;
			std::cout << "my id: " << NetworkHandler::Instance().GetPlayerID() << std::endl;
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
			std::cout << "my id: " << NetworkHandler::Instance().GetPlayerID() << std::endl;
			Refresh(data.players);
			break;
		}
		case MessageType::Ready:
		{
			std::cout << "Got ready message" << std::endl;
			ReadyData data = receivedData.GetData<ReadyData>();
			for (int i = 0; i < data.ids.size(); i++)
			{
				std::cout << "ID: " << data.ids[i] << " Ready: " << data.ready[i] << std::endl;
			}
			*_ids = data.ids;
			if (data.allReady)
			{
				_startReadyTimer = true;
				std::cout << "Seeded" << std::endl;
				srand(data.seed);
			}
			Refresh(data.ids, data.ready);
			break;
		}
		//TODO: have to add in host change here, need host? make it so 4 players required to start and then they ready up
		}
	}

	if (_startReadyTimer)
	{
		_uiSystem.DeleteDisplayTextByID(_countdownTextId);
		_countdownTextId = _uiSystem.CreateDisplayTextColoured("Game starting in....." + std::to_string(_readyTimer), SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85, 255, 0, 0, 255);
		_readyTimer -= dt;
		if (_readyTimer < 0.f)
		{//TODO: this has to be in sync
			std::cout << "Starting game" << std::endl;
			_startingGame = true;
		}
	}



	_lastTime = currentTime;
	if (_startingGame)
		return (int)CurrentScene::GAME;
	else
		return (int)_swapScene;
}

void Lobby::Render()
{
	SDL_RenderClear(_renderer);

	//RENDER HERE
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

	_swapScene = CurrentScene::LOBBY;
}

void Lobby::Stop()
{
	//_inputManager->ResetKey(Event::BACKSPACE);
	//_inputManager->ResetKey(Event::h);

	_running = false;
	CleanUp();
}

void Lobby::OnEvent(Event evt, Type typ)
{
	if (_running)
	{
		switch (typ)
		{
		case Type::Press:
			switch (evt)
			{
			case Event::ESCAPE:
				_running = false;
				break;
			//case Event::w:
			//	_audioManager->PlayFX("Click");
			//	break;
			//case Event::s:
			//	_audioManager->PlayFX("Click");
			//	break;
			}
			break;

		default:
			break;
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
	_inputManager->EmptyKeys();
	_uiSystem.DeleteDisplayText();
	_uiSystem.DeleteText();
	_uiSystem.DeleteEntites();
}

void Lobby::BindInput()
{
	Command* enterIn = new InputCommand([&]()
	{
		if (_selectedItemIndex == _uiSystem.GetInteractiveTextRectangle().size() - 1) { _running = false; }
		else { _swapScene = static_cast<CurrentScene>(_selectedItemIndex + 1); }
	}, Type::Press);

	_inputManager->AddKey(Event::RETURN, enterIn, this);

	Command* mlIn = new InputCommand([&]()
	{
		SDL_Point mousePos = _inputManager->GetMousePos();
		SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

		// Last Button - Create New Lobby
		if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle().back())))
		{
			JoinSessionData data;
			NetworkHandler::Instance().Send(&data);
			std::cout << "Create New Lobby" << std::endl;
		}


		// Lobbies
		for (int i = 0; i < _uiSystem.GetInteractiveTextRectangle().size() - 1; i++)
		{
			if (SDL_HasIntersection(&mouseRect, &(_uiSystem.GetInteractiveTextRectangle()[i])))
			{
				JoinSessionData data;
				data.sessionID = _session[i].id;
				NetworkHandler::Instance().Send(&data);
				std::cout << "ID: " << data.sessionID << std::endl;
			}
		}

	}, Type::Press);

	_inputManager->AddKey(Event::MOUSE_LEFT, mlIn, this);


	Command* rIn = new InputCommand([&]() //ready up
	{
		ReadyData data;
		NetworkHandler::Instance().Send(&data);
	}, Type::Press);
	_inputManager->AddKey(Event::r, rIn, this);

	// Back to Main Menu
	Command* backIn = new InputCommand([&]() { 
		NetworkHandler::Instance().Disconnect();
		_swapScene = Scene::CurrentScene::MAIN_MENU; 
	}, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	// Exit
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
		_selectedItemIndex = _uiSystem.GetInteractiveTextRectangle().size() - 1;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}

void Lobby::MoveDown()
{
	if (_selectedItemIndex < _uiSystem.GetInteractiveTextRectangle().size() - 1)
	{
		_selectedItemIndex++;
	}
	// Jump to top
	else
	{
		_selectedItemIndex = 0;
	}
	_uiSystem.GetDisplayTextRectangle().back().x = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].x + _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].w + 50;
	_uiSystem.GetDisplayTextRectangle().back().y = _uiSystem.GetInteractiveTextRectangle()[_selectedItemIndex].y;
}


int Lobby::GetPressedItem()
{
	// Index in menu
	return _selectedItemIndex;
}

void Lobby::Refresh(const std::vector<Session>& sessions, int maxPlayers)
{
	_uiSystem.DeleteText();
	_uiSystem.DeleteDisplayText();
	_uiSystem.CreateDisplayText("Sessions", SCREEN_WIDTH / 2, 50);
	_uiSystem.CreateDisplayText("________", SCREEN_WIDTH / 2, 60);

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
			_uiSystem.CreateText(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50);
		}
	}

	_session = sessions;

	_uiSystem.CreateTextAtCenter("Create New Lobby", SCREEN_WIDTH / 2, 700);
}

void Lobby::Refresh(const std::vector<int>& players, std::vector<bool> ready)
{
	if (ready.empty())
	{
		ready = std::vector<bool>(players.size(), false);
	}
	_uiSystem.DeleteText();
	_uiSystem.DeleteDisplayText();

	_uiSystem.CreateDisplayText("Players", SCREEN_WIDTH / 2, 50);
	_uiSystem.CreateDisplayText("________", SCREEN_WIDTH / 2, 60);

	for (int i = 0; i < players.size(); i++)
	{
		std::ostringstream oss;
		oss << "Player [" << players[i] << "]";

		if (players[i] == NetworkHandler::Instance().GetPlayerID())
		{
			oss << " (you!).";
		}

		std::string var = oss.str();
		if (i == 0)
		{
			if (ready[i])
			{
				_uiSystem.CreateTextColoured(var, 50, 200, 0, 0, 255, 255);
			}
			else
			{
				_uiSystem.CreateText(var, 50, 200);
			}
		}

		else
		{
			if (ready[i])
			{
				_uiSystem.CreateTextColoured(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50, 0, 0, 255, 255);
			}
			else
			{
				_uiSystem.CreateText(var, 50, _uiSystem.GetInteractiveTextRectangle()[i - 1].y + 50);
			}
		}
	}
}
