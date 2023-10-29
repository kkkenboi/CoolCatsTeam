#include "LitterBox.h"
#include "SceneTestMain.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

LB::Application* LB::CreateApplication()
{
	return DBG_NEW Application();
}

int main(int argc, char** argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

#if (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif

	_crtBreakAlloc = 384;
	_crtBreakAlloc = 71137;
	_crtBreakAlloc = 118965;

	auto app = LB::CreateApplication();
	app->Run();
	delete app;


}
