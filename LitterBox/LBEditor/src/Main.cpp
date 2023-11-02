#include "LitterBox.h"
#include "SceneTestMain.h"

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

	_crtBreakAlloc = 13787; // Systems
	//_crtBreakAlloc = 630; // Serialiser calling DebuggerLog
	//_crtBreakAlloc = 890; // Serialiser allocation	
	//_crtBreakAlloc = 1306; // Debugger constructor
	//_crtBreakAlloc = 6975; // Profiler
	//_crtBreakAlloc = 9196; // Profiler
	//_crtBreakAlloc = 9221; // Profiler destructor
	//_crtBreakAlloc = 10499; // ImGui Allocator
	//_crtBreakAlloc = 10549; // Input ?????
	//_crtBreakAlloc = 10982; // Profiler
	//_crtBreakAlloc = 30360; // Assets

	auto app = LB::CreateApplication();
	app->Run();
	delete app;


#if (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
#endif
}
