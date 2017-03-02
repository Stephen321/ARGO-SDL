#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "Game.h"
#include "ConstHolder.h"
#include "Helpers.h"
#include "LTimer.h"

#include <assert.h>
#include "WeaponComponent.h"
#include "StatusEffectComponent.h"

Game::Game() 
	: _gravity(0.f, 0.f)
	, _world(_gravity)
	, _waypoints()
{
	_running = false;
	_textureHolder = std::map<TextureID, SDL_Texture*>();
}

Game::~Game()
{
}

void Game::Initialize(SDL_Renderer* renderer, const std::vector<int>& ids)
{
	Scene::Initialize(renderer);

	_systemManager.Initialize(_renderer, &_entityFactory, &_bodyFactory, &_world, SCREEN_WIDTH, SCREEN_HEIGHT);


	_world.SetAllowSleeping(false);

	_entityFactory.Initialize(&_textureHolder);
	_bodyFactory.Initialize(&_world);

	Start();
	LoadContent(ids);

	_player = nullptr;

	CreateUI();

	_systemManager.PostInitialize(_player, &_waypoints);

	_swapScene = CurrentScene::GAME;
	BindInput();
}

void Game::LoadContent(const std::vector<int>& ids)
{
	_textureHolder[TextureID::TilemapSpriteSheet] = LoadTexture("Media/Textures/SpriteSheetFull.png");

	_textureHolder[TextureID::Bullet] = LoadTexture("Media/Player/Bullet.png");
	_textureHolder[TextureID::Weapon] = LoadTexture("Media/Player/Weapon.png");
	_textureHolder[TextureID::Flag] = LoadTexture("Media/Player/Flag.png");
	_textureHolder[TextureID::Player] = LoadTexture("Media/Player/playerSS.png");
	_textureHolder[TextureID::Checkpoint] = LoadTexture("Media/Textures/Checkpoint.png");
	_textureHolder[TextureID::PowerUp] = LoadTexture("Media/Textures/PowerUps.png");

	_textureHolder[TextureID::UI] = LoadTexture("Media/UI/UI.png");

	_textureHolder[TextureID::EntitySpriteSheet] = LoadTexture("Media/Textures/EntitySprite.png");

	_levelLoader.LoadJson("Media/Json/NormalMap.json", _systemManager, &_bodyFactory, &_waypoints, ids);

}


int Game::Update()
{
	unsigned int currentTime = LTimer::gameTime();		//millis since game started
	float dt = (float)(currentTime - _lastTime) / 1000.0f;	//time since last update

	// UPDATE HERE //
	// Use yo Update using Poll Event (Menus, single presses)
	_inputManager->ProcessInput();
	// Use to Update constantly at frame rate
	_inputManager->ConstantInput();

	_systemManager.Process(dt);

	_world.Step(1 / (float)SCREEN_FPS, 8, 3);
	//save the curent time for next frame
	_lastTime = currentTime;

	return (int)_swapScene;
}

void Game::Render()
{
	SDL_RenderClear(_renderer);	

	//RENDER HERE
	_systemManager.Render();
	DebugBox2D();

	SDL_RenderPresent(_renderer);
}

bool Game::IsRunning()
{
	if (_swapScene != CurrentScene::GAME) { _swapScene = CurrentScene::GAME; }
	return _running;
}

void Game::Start()
{
	_running = true;
	_swapScene = CurrentScene::GAME;

	if (!_audioManager->IsMusicPlaying())
	{
		int random = std::rand() % 3;
		if (random == 0) { _audioManager->PlayMusic("Music1"); }
		else if (random == 1) { _audioManager->PlayMusic("Music2"); }
		else if (random == 2) { _audioManager->PlayMusic("Music3"); }
	}
}

void Game::Stop()
{
	_running = false;
	CleanUp();
	_audioManager->StopMusic();
}

void Game::OnEvent(Event evt, Type typ)
{
	if (_running)
	{
		switch (typ)
		{
		case Type::Press:
			switch (evt)
			{
			case Event::ESCAPE:
				//_inputManager->saveFile();
				_running = false;
				break;
			}
			break;

		case Type::Down:
			switch (evt)
			{
			case Event::w:
				_audioManager->PlayFX("Hum");
				break;
			case Event::a:
				_audioManager->PlayFX("Hum");
				break;
			case Event::s:
				_audioManager->PlayFX("Hum");
				break;
			case Event::d:
				_audioManager->PlayFX("Hum");
				break;
			}
			break;

		default:
			break;
		}
	}
}


void Game::BindInput()
{
	Command* wIn = new InputCommand(std::bind(&FunctionMaster::MoveVertical, &_functionMaster, -1, _player), Type::Down);
	_inputManager->AddKey(Event::w, wIn, this);

	Command* aIn = new InputCommand(std::bind(&FunctionMaster::MoveHorizontal, &_functionMaster, -1, _player), Type::Down);
	_inputManager->AddKey(Event::a, aIn, this);

	Command* sIn = new InputCommand(std::bind(&FunctionMaster::MoveVertical, &_functionMaster, 1, _player), Type::Down);
	_inputManager->AddKey(Event::s, sIn, this);

	Command* dIn = new InputCommand(std::bind(&FunctionMaster::MoveHorizontal, &_functionMaster, 1, _player), Type::Down);
	_inputManager->AddKey(Event::d, dIn, this);


	// Up
	Command* wUp = new InputCommand(std::bind(&FunctionMaster::MoveVertical, &_functionMaster, 0, _player), Type::Release);
	_inputManager->AddKey(Event::w, wUp, this);

	Command* aUp = new InputCommand(std::bind(&FunctionMaster::MoveHorizontal, &_functionMaster, 0, _player), Type::Release);
	_inputManager->AddKey(Event::a, aUp, this);

	Command* sUp = new InputCommand(std::bind(&FunctionMaster::MoveVertical, &_functionMaster, 0, _player), Type::Release);
	_inputManager->AddKey(Event::s, sUp, this);

	Command* dUp = new InputCommand(std::bind(&FunctionMaster::MoveHorizontal, &_functionMaster, 0, _player), Type::Release);
	_inputManager->AddKey(Event::d, dUp, this);


	// Back to Main Menu
	Command* backIn = new InputCommand([&]() { _swapScene = Scene::CurrentScene::MAIN_MENU; }, Type::Press);
	_inputManager->AddKey(Event::BACKSPACE, backIn, this);

	// Exit Game
	_inputManager->AddListener(Event::ESCAPE, this);

	Command* spaceIn = new InputCommand([&]()
	{
		WeaponComponent* weapon = static_cast<WeaponComponent*>(_player->GetComponent(Component::Type::Weapon));
		
		if ( weapon->hasWeapon)
		{
			if (!static_cast<StatusEffectComponent*>(_player->GetComponent(Component::Type::StatusEffect))->staggered)
			{
				weapon->fired = true;
			}
		}
	}, Type::Hold);
	_inputManager->AddKey(Event::SPACE, spaceIn, this);

}

void Game::CleanUp()
{
	//DESTROY HERE
	_inputManager->EmptyKeys();

	_world.SetAllowSleeping(true);
}

void Game::DebugBox2D()
{
	//DEBUG ASTAR
	
	//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_waypoints.drawNodes(_renderer, &_systemManager.GetCamera());

	_waypoints.drawArcs(_renderer, &_systemManager.GetCamera());
	//DEBUG BOX2D
	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);

	///////////////////use this code for testing purpose///////////////////////////////////////////////////////////////
	for (b2Body* BodyIterator = _world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		if (BodyIterator->IsActive())
		{
			if (BodyIterator->GetUserData())
			{
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
			}
			else
			{
				Entity* e = static_cast<Entity*>(BodyIterator->GetUserData());
				EntityType t = e->GetType();
				if (t == EntityType::Player)
				{
					SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				}
				else if (t == EntityType::Checkpoint)
				{
					SDL_SetRenderDrawColor(_renderer, 127, 255, 212, 255);
				}
				else if (t == EntityType::Point)
				{
					SDL_SetRenderDrawColor(_renderer, 0, 0, 50, 255);
				}
				else if (t == EntityType::Flag)
				{
					SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
				}
				else
				{
					SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
				}
			}

			for (b2Fixture* b2Fixture = BodyIterator->GetFixtureList(); b2Fixture != 0; b2Fixture = b2Fixture->GetNext())
			{
				b2Shape::Type shapeType = b2Fixture->GetType();
				if (shapeType == b2Shape::e_circle)
				{
				}
				else if (shapeType == b2Shape::e_polygon)
				{

					b2PolygonShape* polygonShape = (b2PolygonShape*)b2Fixture->GetShape();

					int lenght = (int)polygonShape->GetVertexCount();

					SDL_Point* points = new SDL_Point[lenght + 1];


					for (int i = 0; i < lenght; i++)
					{
						Point worldPoint;
						float verticesPosX = polygonShape->GetVertex(i).x; b2Fixture->GetBody()->GetPosition().x;
						float verticesPosY = polygonShape->GetVertex(i).y; b2Fixture->GetBody()->GetPosition().y;
						/*
						float mag = sqrt(fixturePosX * fixturePosX + fixturePosY * fixturePosY);
						if (mag != 0)
						{
						fixturePosX /= mag;
						fixturePosY /= mag;
						}*/
						float angle = b2Fixture->GetBody()->GetAngle();
						float s = sin(angle);
						float c = cos(angle);

						// translate point back to origin:
						verticesPosX -= 0;
						verticesPosY -= 0;

						// rotate point
						float xnew = verticesPosX* c - verticesPosY * s;
						float ynew = verticesPosX * s + verticesPosY * c;

						// translate point back:
						verticesPosX = xnew + 0;
						verticesPosY = ynew + 0;

						worldPoint.x = metersToPixels(verticesPosX + b2Fixture->GetBody()->GetPosition().x);
						worldPoint.y = metersToPixels(verticesPosY + b2Fixture->GetBody()->GetPosition().y);
						worldPoint = _systemManager.GetCamera().worldToScreen(worldPoint);
						points[i].x = worldPoint.x;
						points[i].y = worldPoint.y;
					}

					points[lenght].y = points[0].y;
					points[lenght].x = points[0].x;
					
					SDL_RenderDrawLines(_renderer, points, lenght + 1);
					delete points;
				}
			}
		}
	}

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(_renderer);
}

void Game::CreateUI()
{
	// Poll
	std::vector<float> pollPosition1 = std::vector<float>();
	pollPosition1.push_back(0); //id
	pollPosition1.push_back(64); //xPosition
	pollPosition1.push_back(64); //yPosition
	pollPosition1.push_back(64); //width
	pollPosition1.push_back(64); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, pollPosition1));

	std::vector<float> pollPosition2 = std::vector<float>();
	pollPosition2.push_back(2); //id
	pollPosition2.push_back(64); //xPosition
	pollPosition2.push_back(128); //yPosition
	pollPosition2.push_back(64); //width
	pollPosition2.push_back(64); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, pollPosition2));

	std::vector<float> pollPosition3 = std::vector<float>();
	pollPosition3.push_back(3); //id
	pollPosition3.push_back(64); //xPosition
	pollPosition3.push_back(192); //yPosition
	pollPosition3.push_back(64); //width
	pollPosition3.push_back(64); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, pollPosition3));

	// Weapon
	std::vector<float> weapon = std::vector<float>();
	weapon.push_back(4); //id
	weapon.push_back(SCREEN_WIDTH - 128); //xPosition
	weapon.push_back(64); //yPosition
	weapon.push_back(64); //width
	weapon.push_back(64); //height
	_systemManager.AddRequest(std::pair<EntityType, std::vector<float>>(EntityType::UI, weapon));

	// Text
	_systemManager.GetUISystem()->CreateDisplayText("1", 48, 64);
	_systemManager.GetUISystem()->CreateDisplayText("2", 48, 128);
	_systemManager.GetUISystem()->CreateDisplayText("3", 48, 192);

	// Next Checkpoint Text
	_systemManager.GetUISystem()->CreateTextAtCenter("1", 160, 64);
	_systemManager.GetUISystem()->CreateTextAtCenter("1", 160, 128);
	_systemManager.GetUISystem()->CreateTextAtCenter("1", 160, 192);
}

void Game::UpdateUI()
{

}
