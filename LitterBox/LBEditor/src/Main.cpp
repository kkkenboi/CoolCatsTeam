/*!************************************************************************
 \file				Main.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This file handles the creation and updating of the Application and Editor
 instance for users to edit their game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox.h"
#include "Editor/Editor.h"

#include <stdlib.h>
#include <crtdbg.h>

/*!***********************************************************************
\brief
Creates the application in the project area, to be used by other projects
eg. Sandbox
*************************************************************************/
LB::Application* LB::CreateApplication(bool EditorMode, bool startPlaying)
{
	return DBG_NEW Application(EditorMode, startPlaying);
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

	auto app = LB::CreateApplication(editorMode, false);
	auto editor = LB::CreateEditor();
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
