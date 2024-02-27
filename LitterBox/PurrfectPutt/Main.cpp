/*!************************************************************************
 \file				Main.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This file handles the creation and updating of the Application for users
 to play the game!

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox.h"
#include "LitterBox/Core/Core.h"

#include <windows.h>

#include <stdlib.h>
#include <crtdbg.h>

/*!***********************************************************************
\brief
Creates the application in the project area, to be used by other projects
eg. Sandbox
*************************************************************************/
LB::Application* LB::CreateApplication(bool editorMode, bool startPlaying)
{
	return DBG_NEW Application(editorMode, startPlaying);
}

/*!***********************************************************************
  \brief
  Entry point for this program, creates an instance of the Application.
*************************************************************************/
int WINAPI WinMain(_In_ HINSTANCE instanceH, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR command_line, _In_ int show)
{
	bool editorMode = false;

	auto app = LB::CreateApplication(editorMode, true);
	while (app->IsRunning()) 
	{
		LB::TIME->LBFrameStart();
		app->Run();
		LB::TIME->LBFrameEnd();
	}
	delete app;

}
