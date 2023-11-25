/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
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

#include "EditorInspector.h"
#include "Utils/CommandManager.h"
#include "Commands/GameObjectCommands.h"

namespace LB
{
	EditorHierarchy* EDITORHIERACHY = nullptr;

	/*!***********************************************************************
	  \brief
	  Default Constructor for EditorHierarchy
	  \return
	  Nothing.
	*************************************************************************/
	EditorHierarchy::EditorHierarchy(std::string layerName) : Layer(layerName)
	{
		if (!EDITORHIERACHY)
			EDITORHIERACHY = this;
		else
			DebuggerLogError("Editor Hierachy already exist!");
	}

	/*!***********************************************************************
	  \brief
	  Initializes the EditorHierarchy layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorHierarchy::Initialize()
	{
		// Update scenes after startup through events
		SCENEMANAGER->onNewSceneLoad.Subscribe(LB::UpdateSceneLoaded);

		// But first after startup, load the active scene
		UpdateSceneLoaded(SCENEMANAGER->GetCurrentScene());
	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorHierarchy layer.
	  \return
	  Nothing.
	*************************************************************************/
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
			//Spawn GO Command
			std::shared_ptr<SpawnObjectCommand> spawnCommand = std::make_shared<SpawnObjectCommand>();
			COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(spawnCommand));

			// GameObject* newGO = FACTORY->SpawnGameObject();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Selected"))
		{
			if (m_clickedItem)
			{
				// Remove GO command
				std::shared_ptr<RemoveObjectCommand> removeCommand = std::make_shared<RemoveObjectCommand>(EDITORINSPECTOR->GetInspectedGO());
				COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(removeCommand));

				m_clickedItem = nullptr;
				onNewObjectSelected.Invoke(nullptr);
			}
		}

		// Draw the hierarchy for this cene
		DrawRoot();

		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Draws the root of the hierarchy.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorHierarchy::DrawRoot()
	{
		ImGui::PushID(m_loadedScene->GetRoot());

		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_DefaultOpen
			| ((m_loadedScene->GetRoot()->GetChildCount() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

		// If this GO has children GO,
		if (ImGui::TreeNodeEx((m_loadedScene->GetName() + (COMMAND->UpToDate() ? "" : " (*)")).c_str(), flags))
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

	/*!***********************************************************************
	  \brief
	  Draws an item in the hierarchy.
	  \return
	  Nothing.
	*************************************************************************/
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

	/*!***********************************************************************
	  \brief
	  Updates the currently clicked item in the hierarchy.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorHierarchy::UpdateClickedItem(CPTransform* newClickedItem)
	{
		m_clickedItem = newClickedItem;
	}

	/*!***********************************************************************
	  \brief
	  Updates the loaded scene in the hierarchy
	  \return
	  Nothing.
	*************************************************************************/
	void EditorHierarchy::UpdateSceneLoaded(Scene* loadedScene)
	{
		m_loadedScene = loadedScene;
	}

	/*!***********************************************************************
	  \brief
	  Updates the loaded scene.
	  \return
	  Nothing.
	*************************************************************************/
	void UpdateSceneLoaded(Scene* loadedScene)
	{
		EDITORHIERACHY->UpdateSceneLoaded(loadedScene);
	}
}
