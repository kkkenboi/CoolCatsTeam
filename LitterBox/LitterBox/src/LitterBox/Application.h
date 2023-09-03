namespace LitterBox {
	class Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be used in Client
	Application* CreateApplication();

}