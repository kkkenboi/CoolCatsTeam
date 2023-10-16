/*!************************************************************************
 \file				Editor.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions defintions for the Editor where it manages
 all of the different layers of the editor, eg. the Hierarchy layer, the
 Inspector, and more. It ensures that all the layers are updated and
 rendered in order.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "LitterBox/Core/Core.h"

#include "Editor.h"
#include "EditorToolBar.h"
#include "EditorHierarchy.h"
#include "EditorInspector.h"
#include "EditorGameView.h"
#include "EditorSceneView.h"
#include "EditorAssets.h"
#include "EditorConsole.h"
#include "EditorProfiler.h"

#include "Platform/Windows/Windows.h"
#include "LitterBox/Engine/Input.h"

namespace LB
{
	Editor* EDITOR = nullptr;

	void ToggleEditor()
	{
		EDITOR->m_EditorMode = !EDITOR->m_EditorMode;
	}

	Editor::Editor() 
	{
		if (!EDITOR)
		{
			EDITOR = this;
			m_EditorMode = true;
		}
		else
			DebuggerLogError("Editor System already exists!");

		// Add the different ImGui layers in here
		m_ImGuiLayers.AddLayer(new EditorToolBar("ToolBar"));
		m_ImGuiLayers.AddLayer(new EditorHierarchy("Hierarchy"));
		m_ImGuiLayers.AddLayer(new EditorInspector("Inspector"));
		m_ImGuiLayers.AddLayer(new EditorSceneView("Scene View"));
		m_ImGuiLayers.AddLayer(new EditorGameView("Game View"));
		m_ImGuiLayers.AddLayer(new EditorConsole("Console"));
		m_ImGuiLayers.AddLayer(new EditorProfiler("Profiler"));
		m_ImGuiLayers.AddLayer(new EditorAssets("Assets"));
	}

	void Editor::Initialize()
	{
		INPUT->SubscribeToKey(ToggleEditor, KeyCode::KEY_M, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

		// Setting up ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// ImGui flags for docking and viewport
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(WINDOWSSYSTEM->GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");

		// Set Style
		ImGui::StyleColorsDark();
	}

	void Editor::Update()
	{
		if (m_EditorMode)
		{
			// To start every frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					// Docking Section
			static bool dockspaceOpen = true;
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGuiWindowFlags dockingFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNav;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Dockspace", &dockspaceOpen, dockingFlags);
			ImGui::PopStyleVar();

			// Menu Bar 
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{

					if (ImGui::MenuItem("Hide Docking Bar"))
					{
						;
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Exit"))
					{
						MessageQuit q;
						CORE->BroadcastMessage(&q);
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// Dockspace
			if (ImGui::DockBuilderGetNode(ImGui::GetID("MyDockspace")) == NULL)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

				ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
				ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 200000.f, NULL, &dock_main_id);
				//ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
				//ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

				ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
				ImGui::DockBuilderDockWindow("Inspector", dock_main_id);
				ImGui::DockBuilderFinish(dockspace_id);
			}

			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);

			ImGui::End();
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				  //bool open = true;

				  //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				  //ImGuiViewport* viewport = ImGui::GetMainViewport();
				  //ImGui::SetNextWindowPos(viewport->Pos);
				  //ImGui::SetNextWindowSize(viewport->Size);
				  //ImGui::SetNextWindowViewport(viewport->ID);
				  //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				  //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				  //window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				  //window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

				  //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				  //ImGui::Begin("DockSpace Demo", &open, window_flags);
				  //ImGui::PopStyleVar();

				  //ImGui::PopStyleVar(2);

				  //if (ImGui::DockBuilderGetNode(ImGui::GetID("MyDockspace")) == NULL)
				  //{
				  //	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				  //	ImGuiViewport* viewport = ImGui::GetMainViewport();
				  //	ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
				  //	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

				  //	ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
				  //	ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
				  //	ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
				  //	ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

				  //	ImGui::DockBuilderDockWindow("James_1", dock_id_left);
				  //	ImGui::DockBuilderDockWindow("James_2", dock_main_id);
				  //	ImGui::DockBuilderDockWindow("James_3", dock_id_right);
				  //	ImGui::DockBuilderDockWindow("James_4", dock_id_bottom);
				  //	ImGui::DockBuilderFinish(dockspace_id);
				  //}

				  //ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, 0);
				  //ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				  //ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
				  //ImGui::PopStyleColor();
				  //ImGui::End();

				  //ImGui::Begin("James_1", &open, 0);
				  //ImGui::Text("Text 1");
				  //ImGui::End();

				  //ImGui::Begin("James_2", &open, 0);
				  //ImGui::Text("Text 2");
				  //ImGui::End();

				  //ImGui::Begin("James_3", &open, 0);
				  //ImGui::Text("Text 3");
				  //ImGui::End();

				  //ImGui::Begin("James_4", &open, 0);
				  //ImGui::Text("Text 4");
				  //ImGui::End();
				  // Update all the ImGui layers here
			for (Layer* layer : m_ImGuiLayers)
			{
				layer->UpdateLayer();
			}

			// Render Portion
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(WINDOWSSYSTEM->GetWindow());
		}
	}

	void Editor::FixedUpdate()
	{
	}

	void Editor::Destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}