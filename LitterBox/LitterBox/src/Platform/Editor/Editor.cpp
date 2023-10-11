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

#include "Editor.h"
#include "EditorHierarchy.h"
#include "Platform/Windows/Windows.h"
#include "LitterBox/Renderer/Renderer.h"


namespace LB
{
	Editor* EDITOR = nullptr;

	void Editor::Initialize()
	{
		if (!EDITOR)
		{
			EDITOR = this;
		}
		else
		{
			std::cerr << "Editor already exists" << std::endl;
		}

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

		// Add the different ImGui layers in here
		m_ImGuiLayers.AddLayer(new EditorHierarchy("Hierarchy"));
	}

	void Editor::Update()
	{
		// To start every frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Docking Section
		ImGuiWindowFlags dockingFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
										ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNav;
										
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", 0, dockingFlags);

		ImGui::PopStyleVar(0);

		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();

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