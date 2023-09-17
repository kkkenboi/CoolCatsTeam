#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fmod.hpp>



namespace LB {
	Application::Application()
	{
		// Setting up LitterBox Engine with the available systems
		Engine = new LBEngine();

		WindowsSystem* windows = new WindowsSystem();
		Renderer::RenderSystem* graphics = new Renderer::RenderSystem();

		Engine->AddSystem(windows);
		Engine->AddSystem(graphics);
		Engine->AddSystem(new GameObjectFactory());


		Engine->Initialize();

	}
	Application::~Application()
	{
		//Delete all the game objects
		FACTORY->DestroyAllObjects();

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