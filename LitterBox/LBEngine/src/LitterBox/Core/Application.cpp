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

namespace LB {

	/*!***********************************************************************
	 \brief 
	 Initialises the LitterBox Engine with all the systems added
	*************************************************************************/
	Application::Application(Scene* firstScene)
	{
		// Setting up LitterBox Engine with the available systems
		Engine = DBG_NEW LBEngine();

		// TODO: Refactor intialization order and move back to debugger system
		InitializeLoggers();
		Editor*					editor			= DBG_NEW Editor();
		Time*					time			= DBG_NEW Time(100, 60);
		InputSystem*			input			= DBG_NEW InputSystem();
		ProfilerManager*		profiler		= DBG_NEW ProfilerManager();
		WindowsSystem*			windows			= DBG_NEW WindowsSystem();
		Debugger*				debug			= DBG_NEW Debugger();
		FactorySystem*			factory			= DBG_NEW FactorySystem();
		GameObjectManager*		gameManager		= DBG_NEW GameObjectManager();
		GameLogic*				logic			= DBG_NEW GameLogic();
		RigidBodyManager*		physics			= DBG_NEW RigidBodyManager();
		ColliderManager*        colliders       = DBG_NEW ColliderManager();
		AudioManager*			audio			= DBG_NEW AudioManager();
		AssetManager*			assetManager	= DBG_NEW AssetManager();
		Renderer::RenderSystem* graphics		= DBG_NEW Renderer::RenderSystem();
		Memory*					memory			= DBG_NEW Memory();
		SceneManager*			sceneManager	= DBG_NEW SceneManager(firstScene);

		Engine->AddSystem(time);
		Engine->AddSystem(input);
		Engine->AddSystem(profiler);
		Engine->AddSystem(factory);
		Engine->AddSystem(windows);
		Engine->AddSystem(gameManager);
		Engine->AddSystem(assetManager);
		Engine->AddSystem(physics);
		Engine->AddSystem(colliders);
		Engine->AddSystem(logic);
		Engine->AddSystem(audio);
		Engine->AddSystem(memory);
		Engine->AddSystem(editor);
		Engine->AddSystem(graphics);
		Engine->AddSystem(debug);
		Engine->AddSystem(sceneManager);

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

		//Delete the engine itself
		delete Engine;
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
