#include "Platform/Windows/Windows.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Engine/Input.h"
#include "Core.h"

namespace LB {
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		LBEngine* Engine;
	};

	// To be used in Client
	Application* CreateApplication();

}