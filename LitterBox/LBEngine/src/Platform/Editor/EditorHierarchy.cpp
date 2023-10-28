/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions defintions for the hierarchy layer of the
 Editor. This is to ensure that there is a overview of all the game objects
 in the game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "EditorHierarchy.h"

namespace LB
{
	EditorHierarchy* EDITORHIERACHY = nullptr;

	EditorHierarchy::EditorHierarchy(std::string layerName) : Layer(layerName) 
	{
		if (!EDITORHIERACHY)
			EDITORHIERACHY = this;
		else
			DebuggerLogError("Editor Hierachy already exist!");
	}

	void EditorHierarchy::Initialize()
	{
		SCENEMANAGER->onNewSceneLoad.Subscribe(LB::UpdateSceneLoaded);
	}

	void EditorHierarchy::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// If no scene is loaded, don't show anything!
		if (!m_loadedScene) {
			ImGui::End();
			return;
		}

		CPTransform* root = m_loadedScene->GetRoot();
		for (int index{ 0 }; index < root->GetChildCount(); ++index)
		{
			DrawItem(root->GetChild(index));
		}

		if (ImGui::Button("Create Game Object"))
		{
			std::cout << "Hello world :D\n";
		}
		ImGui::End();
	}

	void EditorHierarchy::DrawItem(CPTransform* item)
	{
		ImGui::PushID(item);

		ImGuiTreeNodeFlags flags = 
			ImGuiTreeNodeFlags_OpenOnArrow
			| ((item->GetChildCount() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

		if (ImGui::TreeNodeEx(item->gameObj->GetName().c_str(), flags))
		{
			for (int index{ 0 }; index < item->GetChildCount(); ++index)
			{
				DrawItem(item->GetChild(index));
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	void EditorHierarchy::UpdateSceneLoaded(Scene* loadedScene)
	{
		m_loadedScene = loadedScene;
	}

	void UpdateSceneLoaded(Scene* loadedScene)
	{
		EDITORHIERACHY->UpdateSceneLoaded(loadedScene);
	}
}
