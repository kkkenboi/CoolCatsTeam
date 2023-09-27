#include "LitterBox.h"

#include <LitterBox/Core/EntryPoint.h>

class Editor : public LB::Application
{
public:
	Editor()
	{

	}
	~Editor()
	{

	}
};


LB::Application* LB::CreateApplication()
{
	return new Editor();
}

int main(int argc, char** argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	auto app = LB::CreateApplication();
	app->Run();
	delete app;

}
