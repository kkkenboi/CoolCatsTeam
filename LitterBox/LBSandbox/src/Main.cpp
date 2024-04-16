/*!************************************************************************
 \file				Main.cpp (Sandbox)
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				16/04/2024
 \brief

 This file handles the application to allow for interesting things to happen ^^

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox.h"
#include "Editor/Editor.h"
#include "Utils/Preferences.h"

#include <stdlib.h>
#include <crtdbg.h>

/*!***********************************************************************
\brief
Creates the application in the project area, to be used by other projects
eg. Sandbox
*************************************************************************/
LB::Application* LB::CreateApplication(bool EditorMode)
{
	return DBG_NEW Application(EditorMode);
}

/*!***********************************************************************
  \brief
  Creates an instance of the editor, generally used by the main() function
  for the project.
*************************************************************************/
LB::Editor* LB::CreateEditor()
{
	Renderer::GRAPHICS->turnOnEditorMode();
	return DBG_NEW Editor();
}

/*!***********************************************************************
  \brief
  Entry point for this program, creates an instance of the Application,
  and Editor that wraps the application.
*************************************************************************/
int main(int argc, char** argv)
{

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	bool editorMode = true;

	auto app = LB::CreateApplication(editorMode);
	auto editor = LB::CreateEditor();

	app->Initialise();
	editor->Initialize();

	while (app->IsRunning())
	{
		LB::TIME->LBFrameStart();
		app->Run();
		editor->Run();
		LB::TIME->LBFrameEnd();
	}
	delete editor;
	delete app;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#if (_EDITOR)
#endif
}
