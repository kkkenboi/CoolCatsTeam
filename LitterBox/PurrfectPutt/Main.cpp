#include "LitterBox.h"

#include "LitterBox/Core/Core.h"

#include <windows.h>

#include <stdlib.h>
#include <crtdbg.h>

LB::Application* LB::CreateApplication(bool editorMode)
{
	return DBG_NEW Application(editorMode);
}

int WINAPI WinMain(_In_ HINSTANCE instanceH, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR command_line, _In_ int show)
{
	//UNREFERENCED_PARAMETER(argc);
	//UNREFERENCED_PARAMETER(argv);

	bool editorMode = false;

	int* memleak = new int(4);

	auto app = LB::CreateApplication(editorMode);
	LB::CORE->SetPlayingMode(true);
	while (app->IsRunning()) 
	{
		app->Run();
	}
	delete app;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

}
