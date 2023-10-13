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
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		// TODO: Refactor intialization order and move back to debugger system
		InitializeLoggers();
		Time*					time			= new Time(100, 60);
		InputSystem*			input			= new InputSystem();
		ProfilerManager*		profiler		= new ProfilerManager();
		WindowsSystem*			windows			= new WindowsSystem();
		Debugger*				debug			= new Debugger();
		FactorySystem*			factory			= new FactorySystem();
		GameObjectManager*		gameManager		= new GameObjectManager();
		GameLogic*				logic			= new GameLogic();
		RigidBodyManager*		physics			= new RigidBodyManager();
		AudioManager*			audio			= new AudioManager();
		AssetManager*			assetManager	= new AssetManager();
		Renderer::RenderSystem* graphics		= new Renderer::RenderSystem();
		Memory*					memory			= new Memory();
		SceneManager*			sceneManager	= new SceneManager();
		Editor*					editor			= new Editor();

		Engine->AddSystem(time);
		Engine->AddSystem(input);
		Engine->AddSystem(profiler);
		Engine->AddSystem(factory);
		Engine->AddSystem(windows);
		Engine->AddSystem(gameManager);
		Engine->AddSystem(assetManager);
		Engine->AddSystem(physics);
		Engine->AddSystem(logic);
		Engine->AddSystem(audio);
		Engine->AddSystem(graphics);
		Engine->AddSystem(memory);
		Engine->AddSystem(debug);
		Engine->AddSystem(sceneManager);
		Engine->AddSystem(editor);

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
