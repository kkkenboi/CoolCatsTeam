#include "LitterBox.h"
#include "SceneTestMain.h"

LB::Application* LB::CreateApplication()
{
	return new Application(new SceneTestMain);
}

int main(int argc, char** argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	auto app = LB::CreateApplication();
	app->Run();
	delete app;

}

