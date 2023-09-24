#include "pch.h"
#include "GameLogic.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	void SpawnGameObject()
	{
		// Creating the game object
		GameObject* gameObj = FACTORY->CreateGameObject();
		if (gameObj->GetID() == 0) 		// ID only starts at 1
		{
			gameObj->SetID(FACTORY->GetLastObjID());
		}
		//gameObj->AddComponent(FACTORY->GetCMs()["Physics"]->Create());
		//gameObj->AddComponent(FACTORY->GetCMs()["Transform"]->Create());
		gameObj->AddComponent(FACTORY->GetCMs()["Render"]->Create());
		//gameObj->GetComponents()[0]->Initialise();
		gameObj->StartComponents();

		// Sends game object to the Game Object Manager
		// For now, push back in the function to go manager,
		// However, in the future we might need to change when we need to render a lot in one go
		// or send a equal amount of game objects at one go
		// Might be redundant too because we should initialize a pool at the start
		GOMANAGER->AddGameObject(gameObj);


		std::cout << "Game Object Spawned!!!\n";
	}

	GameLogic* GAMELOGIC = nullptr;

	void GameLogic::Initialize()
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

		INPUT->SubscribeToKey(SpawnGameObject, KeyCode::KEY_P, KeyEvent::PRESSED);
	}

	void GameLogic::Update()
	{
		// Upon keypress of P, spawn one game object
		// Update the game objects
	}

	void GameLogic::Destroy()
	{

	}

}
