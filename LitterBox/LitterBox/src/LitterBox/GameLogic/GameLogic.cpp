#include "pch.h"
#include "GameLogic.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Input.h"

namespace LB
{
	void SpawnGameObject()
	{
		// Creates the game object
		GameObject* gameObj = FACTORY->CreateGameObject();
		FACTORY->AddComponent(gameObj, FACTORY->GetCMs()["Physics"]->Create());
		FACTORY->AddComponent(gameObj, FACTORY->GetCMs()["Transform"]->Create());
		FACTORY->AddComponent(gameObj, FACTORY->GetCMs()["Render"]->Create());

		// Sends game object to the Game Object Manager
		// For now, push back in the function to go manager,
		// However, in the future we might need to change when we need to render a lot in one go
		// or send a equal amount of game objects at one go
		// Might be redundant too because we should initialize a pool at the start
		GOMANAGER->m_GameObjects.push_back(gameObj);

		std::cout << "Game Object Spawned!!!\n";
	}

	GameLogic* GAMELOGIC = nullptr;

	GameLogic::GameLogic()
	{
		// Ensure singleton
		if (!GAMELOGIC)
		{
			GAMELOGIC = this;
		}
		else
		{
			std::cerr << "Game Logic already exist\n";
		}
	}

	void GameLogic::Initialize()
	{
		INPUT->SubscribeToKey(SpawnGameObject, KeyCode::KEY_K, KeyEvent::PRESSED);
	}

	void GameLogic::Update()
	{
		// Upon keypress of K, spawn one game object
		// Update the game objects
	}

	GameLogic::~GameLogic()
	{

	}
}
