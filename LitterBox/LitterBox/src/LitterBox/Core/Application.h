#include "Platform/Windows/Windows.h"
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