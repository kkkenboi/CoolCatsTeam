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


#include "windows.h"
#include <crtdbg.h> 

namespace LB
{
	Editor* EDITOR = nullptr;

	void ToggleEditor()
	{
		CORE->ToggleEditorMode();
	}

	Editor::Editor() 
	{
		if (!EDITOR)
		{
			EDITOR = this;
			CORE->SetEditorMode(true);
			CORE->SetEditorLaunched(true);
		}
		else
			DebuggerLogError("Editor System already exists!");

		SetSystemName("Editor System");
		m_MousePicker = nullptr;

		//_CrtMemCheckpoint(&sOld); //take a snapshot

		// Add the different ImGui layers in here
		//_CrtMemCheckpoint(&sNew); //take a snapshot 
		//if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
		//{
		//	OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		//	_CrtMemDumpStatistics(&sDiff);
		//	OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		//	_CrtMemDumpAllObjectsSince(&sOld);
		//	OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		//	_CrtDumpMemoryLeaks();
		//}

		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorToolBar>("ToolBar")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorInspector>("Inspector")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorHierarchy>("Hierarchy")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorGameView>("Game View")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorSceneView>("Scene View")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorConsole>("Console")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorProfiler>("Profiler")));
		m_ImGuiLayers.AddLayer(std::move(std::make_unique<EditorAssets>("Assets")));
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

		// Create a mouse game object with transform and collider component
		m_MousePicker = FACTORY->SpawnGameObject({ 0.f,0.f });
		m_MousePicker->GetComponent<CPTransform>()->SetScale({ 0.1f,0.1f });

		m_MousePicker->AddComponent(C_CPCollider, FACTORY->GetCMs()[C_CPCollider]->Create());
		m_MousePicker->GetComponent<CPCollider>()->Initialise();
	}

	void Editor::Update()
	{
		if (CORE->IsEditorMode())
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

			ImGuiID toolbarID{};
			ImGuiID consoleID{};
			ImGuiID profilerID{};
			ImGuiID assetsID{};
			ImGuiID sceneviewID{};
			ImGuiID gameviewID{};
			ImGuiID hierarchyID{};
			ImGuiID inspectorID{};

			// Docking Section
			ImGuiID maindockspaceID = ImGui::GetID("MainDockspace");
			if (ImGui::DockBuilderGetNode(ImGui::GetID("MainDockspace")) == NULL)
			{
				// Set IDs for different windows
				// Tabs are based off which layer is added first
				ImGui::DockBuilderRemoveNode(maindockspaceID); // Clear out existing layout
				ImGui::DockBuilderAddNode(maindockspaceID, ImGuiDockNodeFlags_DockSpace); // Add empty node
				ImGui::DockBuilderDockWindow("MainDockspace", maindockspaceID);

				ImGuiID topID{};
				ImGuiID bottomID{};

				// Set toolbar to the top of the screen
				ImGui::DockBuilderSetNodeSize(maindockspaceID, viewport->Size);
				topID = ImGui::DockBuilderSplitNode(maindockspaceID, ImGuiDir_Up, 0.1f, NULL, &bottomID);

				// At the bottom, split console to the top left, and the game view to the bottom left
				toolbarID = ImGui::DockBuilderSplitNode(topID, ImGuiDir_Left, 0.5f, NULL, NULL);
				consoleID = ImGui::DockBuilderSplitNode(bottomID, ImGuiDir_Left, 0.5f, NULL, &assetsID);
				consoleID = ImGui::DockBuilderSplitNode(consoleID, ImGuiDir_Up, 0.5f, NULL, &gameviewID);
				// Assets is set in the bottom middle, hierarchy on the top middle and inspector on the right
				assetsID = ImGui::DockBuilderSplitNode(assetsID, ImGuiDir_Left, 0.5f, NULL, &inspectorID);
				assetsID = ImGui::DockBuilderSplitNode(assetsID, ImGuiDir_Down, 0.5f, NULL, &hierarchyID);

				// Set profiler at the same location as console
				profilerID = consoleID;
				// Set scene view at the same location as game view
				sceneviewID = gameviewID;

				ImGui::DockBuilderDockWindow("ToolBar", toolbarID);
				ImGui::DockBuilderDockWindow("Console", consoleID);
				ImGui::DockBuilderDockWindow("Profiler", profilerID);
				ImGui::DockBuilderDockWindow("Assets", assetsID);
				ImGui::DockBuilderDockWindow("Game View", gameviewID);
				ImGui::DockBuilderDockWindow("Scene View", sceneviewID);
				ImGui::DockBuilderDockWindow("Hierarchy", hierarchyID);
				ImGui::DockBuilderDockWindow("Inspector", inspectorID);

				ImGui::DockBuilderFinish(maindockspaceID);
			}

			ImGui::DockSpace(maindockspaceID, ImVec2(0.0f, 0.0f), 0);

			ImGui::End();


			// Update all the ImGui layers here
			for (std::unique_ptr<Layer>& layer : m_ImGuiLayers)
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

		//for (auto it = m_ImGuiLayers.m_Layers.rbegin(); it != m_ImGuiLayers.m_Layers.rend(); ++it) {
		//	delete* it;
		//}

		m_ImGuiLayers.Destroy();
	}
	
	GameObject* Editor::GetMousePicker()
	{
		return m_MousePicker;
	}

	void Editor::SetObjectPicked(GameObject* obj)
	{
		EDITORINSPECTOR->UpdateInspectedGO(obj);
	}


	void Editor::SetMousePos(Vec2<float> pos)
	{
		m_MousePicker->GetComponent<CPTransform>()->SetPosition(pos);
	}

}