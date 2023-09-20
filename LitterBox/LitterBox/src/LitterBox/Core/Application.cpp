#include "Application.h"

namespace LB {
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		Time*					time	 = new Time();
		InputSystem*			input	 = new InputSystem();
		WindowsSystem*			windows	 = new WindowsSystem();
		Renderer::RenderSystem* graphics = new Renderer::RenderSystem();

		Engine->AddSystem(time);
		Engine->AddSystem(input);
		Engine->AddSystem(windows);
		Engine->AddSystem(graphics);

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