/*!************************************************************************
 \file				Main.cpp
 \author(s)
 \par DP email(s):
 \par Course:
 \date
 \brief

 This file contains

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#if EDITOR_MODE
#include "Editor/Core/Editor.h"
#endif
#include "App/Core/Application.h"

#if EDITOR_MODE
void LaunchEditor()
{
    auto editor{ std::make_unique<LB::Editor>() };
    auto app{ std::make_unique<LB::Application>() };

    app->InitializeEngine();
    editor->InitializeEditor();

    while (app->IsRunning())
    {
        app->NewFrame();

        app->UpdateEngine();
        editor->UpdateEditor();

        app->EndFrame();
    }

    editor->ShutdownEditor();
    app->ShutdownEngine();
}
#else
void LaunchApp()
{
    auto app{ std::make_unique<LB::Application>() };

    app->InitializeEngine();

    while (app->IsRunning())
    {
        app->NewFrame();

        app->UpdateEngine();

        app->EndFrame();

        // DEBUG : TO REMOVE
        app->Quit();
    }

    app->ShutdownEngine();
}
#endif

int main()
{
#if EDITOR_MODE
    LaunchEditor();
#else
    LaunchApp();
#endif
    return 0;
}
