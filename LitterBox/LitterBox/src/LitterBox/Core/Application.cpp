#include "Application.h"

namespace LB {
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		Time*					time	 = new Time();
		InputSystem*			input	 = new InputSystem();
		WindowsSystem*			windows	 = new WindowsSystem();
		GameObjectManager*	gameManager	 = new GameObjectManager();
		FactorySystem*			factory	 = new FactorySystem();
		GameLogic*				logic	 = new GameLogic();
		Renderer::RenderSystem* graphics = new Renderer::RenderSystem();

		Engine->AddSystem(time);
		Engine->AddSystem(input);
		Engine->AddSystem(factory);
		Engine->AddSystem(gameManager);
		Engine->AddSystem(logic);
		Engine->AddSystem(windows);
		Engine->AddSystem(graphics);

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