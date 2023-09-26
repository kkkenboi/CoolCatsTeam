#include "Application.h"

namespace LB {
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		Time*					time		= new Time();
		InputSystem*			input		= new InputSystem();
		ProfilerManager*		profiler	= new ProfilerManager();
		WindowsSystem*			windows		= new WindowsSystem();
		Renderer::RenderSystem* graphics	= new Renderer::RenderSystem();
		GameObjectManager*		gameManager	= new GameObjectManager();
		FactorySystem*			factory		= new FactorySystem();
		GameLogic*				logic		= new GameLogic();
		Memory*					memory		= new Memory();

		Engine->AddSystem(time);
		Engine->AddSystem(input);
		Engine->AddSystem(profiler);
		Engine->AddSystem(factory);
		Engine->AddSystem(windows);
		Engine->AddSystem(graphics);
		Engine->AddSystem(gameManager);
		Engine->AddSystem(logic);
		Engine->AddSystem(memory);

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