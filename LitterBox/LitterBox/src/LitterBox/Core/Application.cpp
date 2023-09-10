#include "Application.h"

namespace LB {
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		WindowsSystem* windows = new WindowsSystem();

		Engine->AddSystem(windows);

		Engine->Initialize();

	}
	Application::~Application()
	{
		////Delete all the game objects
		//FACTORY->DestroyAllObjects();

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