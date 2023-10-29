/*!************************************************************************
 \file				Application.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions definitions of the Application class that
 sets up the engine functionalities. This is to allow it to be used on
 other projects.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or 
 disclosure of this file or its contents without the prior written consent 
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Application.h"

#include "windows.h"
#include <crtdbg.h> 
#include <memory>

namespace LB {

	/*!***********************************************************************
	 \brief 
	 Initialises the LitterBox Engine with all the systems added
	*************************************************************************/
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = std::make_unique<LBEngine>();

		// TODO: Refactor intialization order and move back to debugger system
		InitializeLoggers();


		//_CrtMemState sOld;
		//_CrtMemState sNew;
		//_CrtMemState sDiff;

		//_CrtMemCheckpoint(&sOld); //take a snapshot

		std::unique_ptr<Editor>	editor = std::make_unique<Editor>();

		//_CrtMemCheckpoint(&sNew); //take a snapshot 
		//if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
		//{
		//	OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		//	_CrtMemDumpStatistics(&sDiff);
		//	OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		//	_CrtMemDumpAllObjectsSince(&sOld);
		//	OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		//	_CrtDumpMemoryLeaks();
		//}

		std::unique_ptr<Time> time = std::make_unique<Time>(100, 60);
		std::unique_ptr<InputSystem> input = std::make_unique<InputSystem>();
		std::unique_ptr<ProfilerManager> profiler = std::make_unique<ProfilerManager>();
		std::unique_ptr<WindowsSystem> windows = std::make_unique<WindowsSystem>();
		std::unique_ptr<Debugger> debug = std::make_unique<Debugger>();
		std::unique_ptr<FactorySystem> factory = std::make_unique<FactorySystem>();
		std::unique_ptr<GameObjectManager> gameManager = std::make_unique<GameObjectManager>();
		std::unique_ptr<GameLogic> logic = std::make_unique<GameLogic>();
		std::unique_ptr<CPPGameLogic> cppLogic = std::make_unique<CPPGameLogic>();
		std::unique_ptr<RigidBodyManager> physics = std::make_unique<RigidBodyManager>();
		std::unique_ptr<ColliderManager> colliders = std::make_unique<ColliderManager>();
		std::unique_ptr<AudioManager> audio = std::make_unique<AudioManager>();
		std::unique_ptr<AssetManager> assetManager = std::make_unique<AssetManager>();
		std::unique_ptr<Renderer::RenderSystem> graphics = std::make_unique<Renderer::RenderSystem>();
		std::unique_ptr<Memory> memory = std::make_unique<Memory>();
		std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager>();


		//Time*					time			= DBG_NEW Time(100, 60);
		//InputSystem*			input			= DBG_NEW InputSystem();
		//ProfilerManager*		profiler		= DBG_NEW ProfilerManager();
		//WindowsSystem*			windows			= DBG_NEW WindowsSystem();
		//Debugger*				debug			= DBG_NEW Debugger();
		//FactorySystem*			factory			= DBG_NEW FactorySystem();
		//GameObjectManager*		gameManager		= DBG_NEW GameObjectManager();
		//GameLogic*				logic			= DBG_NEW GameLogic();
		//RigidBodyManager*		physics			= DBG_NEW RigidBodyManager();
		//ColliderManager*        colliders       = DBG_NEW ColliderManager();
		//AudioManager*			audio			= DBG_NEW AudioManager();
		//AssetManager*			assetManager	= DBG_NEW AssetManager();
		//Renderer::RenderSystem* graphics		= DBG_NEW Renderer::RenderSystem();
		//Memory*					memory			= DBG_NEW Memory();
		//SceneManager*			sceneManager	= DBG_NEW SceneManager();

		Engine->AddSystem(std::move(time));
		Engine->AddSystem(std::move(input));
		Engine->AddSystem(std::move(profiler));
		Engine->AddSystem(std::move(factory));
		Engine->AddSystem(std::move(windows));
		Engine->AddSystem(std::move(gameManager));
		Engine->AddSystem(std::move(assetManager));
		Engine->AddSystem(std::move(physics));
		Engine->AddSystem(std::move(colliders));
		Engine->AddSystem(std::move(logic));
		Engine->AddSystem(std::move(cppLogic));
		Engine->AddSystem(std::move(audio));
		Engine->AddSystem(std::move(memory));
		Engine->AddSystem(std::move(editor));
		Engine->AddSystem(std::move(graphics));
		Engine->AddSystem(std::move(debug));
		Engine->AddSystem(std::move(sceneManager));

		Engine->Initialise();

	}

	/*!***********************************************************************
	 \brief
	 Destroys the systems and the LitterBox Engine itself
	*************************************************************************/
	Application::~Application()
	{
		//Delete all the systems
		Engine->DestroySystems();

		// TODO: Refactor intialization order and move back to debugger system
		FlushDebugLog();

		//Engine deletes itself here
	}

	/*!***********************************************************************
	 \brief
	 Keeps the LitterBox Engine game loop running if program is not closing

	 \return
	 Nothing
	*************************************************************************/
	void Application::Run()
	{
		while (Engine->IsRunning())
		{
			Engine->GameLoop();
		}
	}
}
