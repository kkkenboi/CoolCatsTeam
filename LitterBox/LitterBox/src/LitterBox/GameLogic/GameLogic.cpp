#include "pch.h"
#include "GameLogic.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"

//REMOVE AFTERWARDS
#include "LitterBox/Renderer/Renderer.h"
extern Renderer::RenderSystem* Renderer::GRAPHICS;

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
		gameObj->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
		gameObj->AddComponent("CPRender", FACTORY->GetCMs()["CPRender"]->Create());
		gameObj->AddComponent("CPRigidBody", FACTORY->GetCMs()["CPRigidBody"]->Create());
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

	//Objects for graphics
	GameObject* scaleObj;
	GameObject* rotObj;
	GameObject* animObj;

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
	GameLogic::~GameLogic()
	{

	}

	void GameLogic::Initialize()
	{
		SetSystemName("Game Logic System");
		INPUT->SubscribeToKey(SpawnGameObject, KeyCode::KEY_P, KeyEvent::PRESSED);

		//---------------------------------scale object--------------------------------
		//// Creating the game object
		//scaleObj = FACTORY->CreateGameObject();
		//if (scaleObj->GetID() == 0) 		// ID only starts at 1
		//{
		//	scaleObj->SetID(FACTORY->GetLastObjID());
		//}
		//scaleObj->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
		//scaleObj->AddComponent("CPRender", FACTORY->GetCMs()["CPRender"]->Create());
		////scaleObj->AddComponent("CPRigidBody", FACTORY->GetCMs()["CPRigidBody"]->Create());
		//scaleObj->StartComponents();

		//GOMANAGER->AddGameObject(scaleObj);
		////---------------------------------scale object--------------------------------

		//-----------------------------Rotation object--------------------------------
		// Creating the game object
		//rotObj = FACTORY->CreateGameObject();
		//if (rotObj->GetID() == 0) 		// ID only starts at 1
		//{
		//	rotObj->SetID(FACTORY->GetLastObjID());
		//}
		//rotObj->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
		//rotObj->AddComponent("CPRender", FACTORY->GetCMs()["CPRender"]->Create());
		////rotObj->AddComponent("CPRigidBody", FACTORY->GetCMs()["CPRigidBody"]->Create());
		//rotObj->StartComponents();

		//GOMANAGER->AddGameObject(rotObj);
		////-----------------------------Rotation object--------------------------------

		////-----------------------------Animation object--------------------------------
		//// Creating the game object
		//animObj = FACTORY->CreateGameObject();
		//if (animObj->GetID() == 0) 		// ID only starts at 1
		//{
		//	animObj->SetID(FACTORY->GetLastObjID());
		//}
		//animObj->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
		//animObj->AddComponent("CPRender", FACTORY->GetCMs()["CPRender"]->Create());
		////animObj->AddComponent("CPRigidBody", FACTORY->GetCMs()["CPRigidBody"]->Create());
		//animObj->StartComponents();

		//GOMANAGER->AddGameObject(animObj);
		////-----------------------------Animation object--------------------------------
		//rotObj->GetComponent<CPTransform>("CPTransform")->SetPosition({ 300.f, 450.f });
		//rotObj->GetComponent<CPTransform>("CPTransform")->SetScale({ 1.f, 1.f });
		//scaleObj->GetComponent<CPTransform>("CPTransform")->SetPosition({ 600.f, 450.f });
	}

	void GameLogic::Update()
	{
		// Upon keypress of P, spawn one game object
		// Update the game objects

		//scale and rotate objects to show it can be done dynamically
		//TODO update the transform later
		//static float degree{ 0.f };
		//scaleObj->GetComponent<CPTransform>("CPTransform")->SetScale({ sinf(degree) ,sinf(degree) });
		//rotObj->GetComponent<CPTransform>("CPTransform")->SetRotation(degree);

		//degree = degree > 6.28318531f ? 0.f : degree + 0.01f;
	}

	void GameLogic::Destroy()
	{

	}
}
