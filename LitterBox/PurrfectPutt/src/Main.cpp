#include "LitterBox.h"

#include "LitterBox/Core/Core.h"

#include <windows.h>

#include <stdlib.h>
#include <crtdbg.h>

LB::Application* LB::CreateApplication()
{
	return DBG_NEW Application();
}

int WINAPI WinMain(_In_ HINSTANCE instanceH, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR command_line, _In_ int show)
{
	//UNREFERENCED_PARAMETER(argc);
	//UNREFERENCED_PARAMETER(argv);

	auto app = LB::CreateApplication();
	LB::CORE->SetPlayingMode(true);
	while (app->IsRunning()) 
	{
		app->Run();
	}
	delete app;

}
