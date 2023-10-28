#include "LitterBox.h"
#include "SceneTestMain.h"

LB::Application* LB::CreateApplication()
{
	return new Application();
}

int main(int argc, char** argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	//#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//#endif

	auto app = LB::CreateApplication();
	app->Run();
	delete app;

}
