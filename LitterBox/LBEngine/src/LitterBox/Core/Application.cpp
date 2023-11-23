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
	Application::Application(bool editorMode)
	{
		// Setting up LitterBox Engine with the available systems
		Engine = std::make_shared<LBEngine>();
		Engine->SetEditorMode(editorMode);

		// TODO: Refactor intialization order and move back to debugger system
		InitializeLoggers();

		std::shared_ptr<Time> time = std::make_shared<Time>(100, 60);
		std::shared_ptr<InputSystem> input = std::make_shared<InputSystem>();
		std::shared_ptr<ProfilerManager> profiler = std::make_shared<ProfilerManager>();
		std::shared_ptr<Debugger> debug = std::make_shared<Debugger>();
		std::shared_ptr<WindowsSystem> windows = std::make_shared<WindowsSystem>();
		std::shared_ptr<FactorySystem> factory = std::make_shared<FactorySystem>();
		std::shared_ptr<GameObjectManager> gameManager = std::make_shared<GameObjectManager>();
		std::shared_ptr<GameLogic> logic = std::make_shared<GameLogic>();
		std::shared_ptr<CPPGameLogic> cppLogic = std::make_shared<CPPGameLogic>();
		std::shared_ptr<RigidBodyManager> physics = std::make_shared<RigidBodyManager>();
		std::shared_ptr<ColliderManager> colliders = std::make_shared<ColliderManager>();
		std::shared_ptr<AudioManager> audio = std::make_shared<AudioManager>();
		std::shared_ptr<AssetManager> assetManager = std::make_shared<AssetManager>();
		std::shared_ptr<Renderer::RenderSystem> graphics = std::make_shared<Renderer::RenderSystem>();
		std::shared_ptr<Memory> memory = std::make_shared<Memory>();
		std::shared_ptr<SceneManager> sceneManager = std::make_shared<SceneManager>();

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
		Engine->AddSystem(cppLogic);
		Engine->AddSystem(audio);
		Engine->AddSystem(memory);
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
	}

	/*!***********************************************************************
	 \brief
	 Keeps the LitterBox Engine game loop running if program is not closing

	 \return
	 Nothing
	*************************************************************************/
	void Application::Run()
	{
		Engine->GameLoop();

		/*while (Engine->IsRunning())
		{
			Engine->GameLoop();
		}*/
	}

	bool Application::IsRunning()
	{
		return Engine->IsRunning();
	}
}
