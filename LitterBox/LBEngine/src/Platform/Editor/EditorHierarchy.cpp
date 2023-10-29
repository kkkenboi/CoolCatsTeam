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

		if (ImGui::Button("Create Game Object"))
		{
			GameObject* newGO = FACTORY->SpawnGameObject();

			newGO->GetComponent<CPTransform>()->SetParent(m_loadedScene->GetRoot());
			m_loadedScene->GetRoot()->AddChild(newGO->GetComponent<CPTransform>());
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Selected"))
		{
			if (m_clickedItem)
			{
				GOMANAGER->RemoveGameObject(m_clickedItem->gameObj);
				m_clickedItem = nullptr;
				onNewObjectSelected.Invoke(nullptr);
			}
		}

		// Draw the hierarchy for this cene
		DrawRoot();

		ImGui::ShowDemoWindow();

		ImGui::End();
	}

	void EditorHierarchy::DrawRoot()
	{
		ImGui::PushID(m_loadedScene->GetRoot());

		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_DefaultOpen
			| ((m_loadedScene->GetRoot()->GetChildCount() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

		// If this GO has children GO,
		if (ImGui::TreeNodeEx(m_loadedScene->GetName().c_str(), flags))
		{
			// Recursively render each one
			for (int index{ 0 }; index < m_loadedScene->GetRoot()->GetChildCount(); ++index)
			{
				DrawItem(m_loadedScene->GetRoot()->GetChild(index));
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	bool EditorHierarchy::DrawItem(CPTransform* item)
	{
		//-------------------------Click Select Detection-------------------------
		// IMGui click detection is weird, but checking click in 3 places does the trick!
		bool isChildClicked{ false }, isItemClicked{ false }, isParentClicked{ false };

		ImGui::PushID(item);

		//-------------------------Item Display Flags-------------------------
		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_OpenOnArrow
			| ((item->GetChildCount() == 0) ? ImGuiTreeNodeFlags_Leaf : 0)
			| ImGuiTreeNodeFlags_DefaultOpen;

		if (item == m_clickedItem)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		//-------------------------Item Display Flags-------------------------

		// First click check before going into the children
		if (ImGui::IsItemClicked()) isParentClicked = true;

		//-------------------------Tree Node Item Render-------------------------
		// If this GO has children GO,
		if (ImGui::TreeNodeEx(item->gameObj->GetName().c_str(), flags))
		{
			// Recursively render each one
			for (int index{ 0 }; index < item->GetChildCount(); ++index)
			{
				// Second click check from the children
				isChildClicked = DrawItem(item->GetChild(index));
			}

			ImGui::TreePop();
		}
		//-------------------------Tree Node Item Render-------------------------

		// Last click check after going into the children
		if (ImGui::IsItemClicked()) isItemClicked = true;

		//-------------------------Click Select Detection-------------------------
		// If this child GO is clicked on,
		if (isItemClicked && !isChildClicked)
		{
			// Update the item clicked (for highlighting in hierachy)
			m_clickedItem = item;
			// Tell the editor know a new GO has been selected
			onNewObjectSelected.Invoke(item->gameObj);
		}
		// Else, ImGui::IsClicked selects the next item, so go back 1 item
		else if (isParentClicked && !item->GetChildCount())
		{
			if (item->GetParent()->gameObj)
			{
				// Update the item clicked (for highlighting in hierachy)
				m_clickedItem = item->GetParent();
				// Tell the editor know a new GO has been selected
				onNewObjectSelected.Invoke(item->GetParent()->gameObj);
			}
		}
		//-------------------------Click Select Detection-------------------------
		ImGui::PopID();

		return isItemClicked;
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
