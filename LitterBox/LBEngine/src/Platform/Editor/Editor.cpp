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

#include "implot.h"

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
			m_onLaunch = true;
		}
		else
			DebuggerLogError("Editor System already exists!");

		SetSystemName("Editor System");

		m_GameObjectPointer = nullptr;

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
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// ImGui flags for docking and viewport
		io.ConfigFlags |= ImGuiWindowFlags_MenuBar;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(WINDOWSSYSTEM->GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");

		// Set Style
		ImGui::StyleColorsDark();

		// Call Initialize for all layers in the layerstack
		m_ImGuiLayers.InitializeLayers();
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

			ImGuiWindowFlags dockingFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
											ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav;

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
			if (m_onLaunch)
			{
				if (ImGui::DockBuilderGetNode(ImGui::GetID("MyDockspace")) == NULL)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
					ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
					ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node

					ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
					ImGuiID dock_id_topbar = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, NULL, &dock_main_id);
					ImGuiID dock_id_top = ImGui::DockBuilderSplitNode(dock_id_topbar, ImGuiDir_Up, 0.20f, NULL, &dock_id_topbar);
					ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_id_topbar, ImGuiDir_Left, 0.20f, NULL, &dock_id_topbar);
					ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_id_topbar, ImGuiDir_Right, 0.20f, NULL, &dock_id_topbar);
					ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_id_topbar, ImGuiDir_Down, 0.20f, NULL, &dock_id_topbar);

					ImGui::DockBuilderDockWindow("Hierarchy", dock_id_right);
					ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
					ImGui::DockBuilderDockWindow("ToolBar", dock_main_id);
					ImGui::DockBuilderDockWindow("Scene View", dock_id_left);
					ImGui::DockBuilderDockWindow("Game View", dock_id_left);
					ImGui::DockBuilderDockWindow("Console", dock_id_bottom);
					ImGui::DockBuilderDockWindow("Profiler", dock_id_bottom);
					ImGui::DockBuilderDockWindow("Assets", dock_id_bottom);

					ImGui::DockBuilderFinish(dockspace_id);
				}

				ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
			}

			ImGui::End();
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			////set default docking positions, may need to use serialization to set first launch
			//if (m_onLaunch)
			//{
			//	//after setting first dock positions
			//	m_onLaunch = false;

			//	//start dock
			//	ImGui::DockBuilderRemoveNode(dockspace_id);
			//	ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
			//	ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			//	//Imgui docks right side by default
			//	ImGui::DockBuilderDockWindow("sceneview", dockspace_id);

			//	//set the other sides
			//	ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.3f, nullptr, &dockspace_id);
			//	ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, &dockspace_id, &dockspace_id);
			//	ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.3f, &dockspace_id, &dockspace_id);

			//	//setting the other dock locations
			//	ImGui::DockBuilderDockWindow("objectlistwindow", dock_id_right);

			//	//set on the save location to dock ontop of eachother
			//	ImGui::DockBuilderDockWindow("resourcewindow", dock_id_down);
			//	ImGui::DockBuilderDockWindow("consolewindow", dock_id_down);


			//	//set on the save location to dock ontop of eachother
			//	ImGui::DockBuilderDockWindow("componentwindow", dock_id_left);

			//	//split the bottom into 2
			//	ImGuiID dock_id_down2 = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Right, 0.5f, nullptr, &dock_id_down);

			//	ImGui::DockBuilderDockWindow("logwindow", dock_id_down2);

			//	//end dock
			//	ImGui::DockBuilderFinish(dockspace_id);

			//}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	GameObject* Editor::InspectedGO()
	{
		return m_GameObjectPointer;
	}

	void Editor::InspectGO(GameObject* go)
	{
		m_GameObjectPointer = go;
	}

}