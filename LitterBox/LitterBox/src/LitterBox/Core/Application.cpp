#include "Application.h"

namespace LB {
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		Time*					time			= new Time();
		InputSystem*			input			= new InputSystem();
		ProfilerManager*		profiler		= new ProfilerManager();
		FactorySystem*			factory			= new FactorySystem();
		WindowsSystem*			windows			= new WindowsSystem();
		GameObjectManager*		gameManager		= new GameObjectManager();
		GameLogic*				logic			= new GameLogic();
		RigidBodyManager*		physics			= new RigidBodyManager();
		AudioManager*			audio			= new AudioManager();
		AssetManager*			assetManager	= new AssetManager();
		Renderer::RenderSystem* graphics		= new Renderer::RenderSystem();
		Memory*					memory			= new Memory();
		Debugger*				debug			= new Debugger();
		SceneManager*			sceneManager	= new SceneManager();

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

		Engine->Initialize();

	}
	Application::~Application()
	{
		////Delete all the game objects
		//FACTORY->DestroyAllObjects();
		//delete GOMANAGER->m_GameObjects;

		//Delete all the systems
		Engine->DestroySystems();

		//Delete the engine itself
		delete Engine;

		//Game over, application will now close
	}

	void Application::Run()
	{
		while (Engine->IsRunning())
		{
			Engine->GameLoop();
		}
	}
}