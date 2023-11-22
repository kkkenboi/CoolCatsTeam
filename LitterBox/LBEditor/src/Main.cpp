#include "LitterBox.h"
#include "Editor/Editor.h"

#include <stdlib.h>
#include <crtdbg.h>

LB::Application* LB::CreateApplication()
{
	return DBG_NEW Application();
}

LB::Editor* LB::CreateEditor()
{
	Renderer::GRAPHICS->turnOnEditorMode();
	return DBG_NEW Editor();
}

int main(int argc, char** argv)
{

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	auto app = LB::CreateApplication();
	auto editor = LB::CreateEditor();
	while (app->IsRunning()) 
	{
		app->Run();
		editor->Run();
	}
	delete editor;
	delete app;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#if (_EDITOR)
#endif
}
