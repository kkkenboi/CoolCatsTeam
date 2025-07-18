/*!************************************************************************
 \file				Editor.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
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
#include "EditorPrefabWindow.h"
#include "EditorAnimationEditor.h"
#include "EditorCollisionLayer.h"
#include "EditorTMGridView.h"
#include "EditorRenderLayer.h"
#include "EditorCollisionGrid.h"
#include "EditorAnimationView.h"

#include "Platform/Windows/Windows.h"
#include "LitterBox/Engine/Input.h"

#include "Utils/Preferences.h"

#include "implot.h"
#include "ImGuizmo.h"

#include "windows.h"
#include <crtdbg.h> 

namespace LB
{
	Editor* EDITOR = nullptr;

	/*!***********************************************************************
	  \brief
	  Switches from editor mode to game mode.
	  \return
	  Nothing.
	*************************************************************************/
	void ToggleEditor()
	{
		CORE->ToggleEditorMode();
	}

	void UpdateInspectedScene(Scene* newScene)
	{
		Preferences::Instance()->m_inspectedScene = newScene->GetName();
	}

	/*!***********************************************************************
	  \brief
	  Constructor for the Editor class.
	  \return
	  Nothing.
	*************************************************************************/
	Editor::Editor()
	{
		if (!EDITOR)
		{
			EDITOR = this;
			CORE->SetEditorMode(true);
		}
		else
			DebuggerLogError("Editor System already exists!");

		// Load preferences
		Preferences::Instance()->LoadPreferences();
		SCENEMANAGER->m_firstScene = Preferences::Instance()->m_inspectedScene;

		// Initialize command manager
		commandManager = std::make_shared<CommandManager>();

		// Construct editor layers
		EditorAnimationEditor::InitializeSingleton("Animation Editor");
		EditorAnimationView::InitializeSingleton("Animation View");

		// Initialize editor layers
		m_ImGuiLayers.AddLayer(std::make_shared<EditorToolBar>("ToolBar"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorInspector>("Inspector"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorHierarchy>("Hierarchy"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorGameView>("Game View"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorSceneView>("Scene View"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorConsole>("Console"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorProfiler>("Profiler"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorAssets>("Assets"));
		m_ImGuiLayers.AddLayer(EditorAnimationEditor::Instance());
		m_ImGuiLayers.AddLayer(EditorAnimationView::Instance());
		m_ImGuiLayers.AddLayer(std::make_shared<EditorPrefabWindow>("Prefab Viewer"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorCollisionLayer>("Collision Layers"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorTMGridView>("TileMap Editor"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorRenderLayer>("Render Layers"));
		m_ImGuiLayers.AddLayer(std::make_shared<EditorCollisionGrid>("Collision Grid"));

		//Initialize();
	}

	/*!***********************************************************************
	  \brief
	  Calls Update, run is called every frame in the Main function.
	  \return
	  Nothing.
	*************************************************************************/
	void Editor::Run()
	{
		Update();
	}

	/*!***********************************************************************
	  \brief
	  Initializes the Editor system.
	  \return
	  Nothing.
	*************************************************************************/
	void Editor::Initialize()
	{
		// Do not re-enable until M3!
		//INPUT->SubscribeToKey(ToggleEditor, KeyCode::KEY_M, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

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
		std::filesystem::path iniLocation(ASSETMANAGER->_appData / ASSETMANAGER->folderName / Preferences::Instance()->m_editoriniFile);
		// For new users, if the ini does not exist, copy the default ini in Editor/Layouts
		if (!std::filesystem::exists(iniLocation))
		{
			std::filesystem::path defaultIni("Editor/Layouts/Default.ini");
			std::filesystem::copy_file(defaultIni, iniLocation, std::filesystem::copy_options::overwrite_existing);
		}

		Preferences::Instance()->m_editoriniFilePath = iniLocation.string();
		ImGui::GetIO().IniFilename = Preferences::Instance()->m_editoriniFilePath.c_str();

		ImGui::StyleColorsDark();

		SCENEMANAGER->onNewSceneLoad.Subscribe(&UpdateInspectedScene);

		// Call Initialize for all layers in the layerstack
		m_ImGuiLayers.InitializeLayers();
	}

	/*!***********************************************************************
	  \brief
	  Updates the Editor system.
	  \return
	  Nothing.
	*************************************************************************/
	void Editor::Update()
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
										ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dockspace", &dockspaceOpen, dockingFlags);
		ImGui::PopStyleVar();

		// Menu Bar 
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Save"))
				{
					SCENEMANAGER->TrySaveScene();
				}
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
		ImGuiID animationID{};
		ImGuiID prefabID{};
		ImGuiID collisionLayerID{};
		ImGuiID renderLayerID{};
		ImGuiID collisionGridID{};
		ImGuiID tileMapID{};
		ImGuiID animViewID{};
		ImGuiID animEditorID{};

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
			inspectorID = ImGui::DockBuilderSplitNode(inspectorID, ImGuiDir_Up, 0.489f, NULL, &prefabID);

			// Set profiler at the same location as console
			profilerID = consoleID;
			// Set scene view at the same location as console
			sceneviewID = consoleID;
			// Set the tile map at the same location as the console
			tileMapID = consoleID;
			// Set the animation at the same location as game view
			animationID = gameviewID;
			// Set the animation editor at the same location as the game view
			animEditorID = gameviewID;
			// Set the collision layer at the same location as the assets
			collisionLayerID = assetsID;
			// Set the collision grid at the same location as the assets
			collisionGridID = assetsID;
			// Set the animation view at the same location as the prefab
			animViewID = prefabID;


			ImGui::DockBuilderDockWindow("ToolBar", toolbarID);
			ImGui::DockBuilderDockWindow("Console", consoleID);
			ImGui::DockBuilderDockWindow("Profiler", profilerID);
			ImGui::DockBuilderDockWindow("Assets", assetsID);
			ImGui::DockBuilderDockWindow("Game View", gameviewID);
			ImGui::DockBuilderDockWindow("Scene View", sceneviewID);
			ImGui::DockBuilderDockWindow("Hierarchy", hierarchyID);
			ImGui::DockBuilderDockWindow("Inspector", inspectorID);
			ImGui::DockBuilderDockWindow("Animation Editor", animationID);
			ImGui::DockBuilderDockWindow("Prefab Viewer", prefabID);
			ImGui::DockBuilderDockWindow("Collision Layers", collisionLayerID);
			ImGui::DockBuilderDockWindow("Render Layers", renderLayerID);
			ImGui::DockBuilderDockWindow("Collision Grid", collisionGridID);
			ImGui::DockBuilderDockWindow("TileMap Editor", collisionGridID);
			ImGui::DockBuilderDockWindow("Animation View", collisionGridID);
			ImGui::DockBuilderDockWindow("Animation Editor", collisionGridID);


			ImGui::DockBuilderFinish(maindockspaceID);
		}

		ImGui::DockSpace(maindockspaceID, ImVec2(0.0f, 0.0f), 0);

		ImGui::End();

		// Update all the ImGui layers here
		for (std::shared_ptr<Layer>& layer : m_ImGuiLayers)
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

	/*!***********************************************************************
	  \brief
	  Destroys the Editor system.
	  \return
	  Nothing.
	*************************************************************************/
	void Editor::Destroy()
	{		
		// Save preferences
		Preferences::Instance()->SavePreferences();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		m_ImGuiLayers.Destroy();
	}

	/*!***********************************************************************
	  \brief
	  Calls Destroy when the editor is deleted to free up all resources.
	*************************************************************************/
	Editor::~Editor()
	{
		Destroy();
	}
}