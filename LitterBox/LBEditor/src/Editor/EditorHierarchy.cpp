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
		
		INPUT->SubscribeToKey(LB::DeleteSelectedObject, KeyCode::KEY_DELETE, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

		// But first after startup, load the active scene
		UpdateSceneLoaded(SCENEMANAGER->GetCurrentScene());

		// On any scene gameobject destroy during gameplay, deselect any
		GOMANAGER->onGameObjectDestroy.Subscribe(LB::CheckGameObjectDeleted);
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
				std::shared_ptr<RemoveObjectCommand> removeCommand = std::make_shared<RemoveObjectCommand>(InspectorGameObject::Instance()->GetInspectedGO());
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

		bool isOpen{ ImGui::TreeNodeEx((m_loadedScene->GetName() + (COMMAND->UpToDate() ? "" : " (*)")).c_str(), flags) };

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* objData = ImGui::AcceptDragDropPayload("HIERARCHY_OBJ"))
			{
				m_draggedItem->SetParent(m_loadedScene->GetRoot());
				m_draggedItem = nullptr;
			}
		}

		// If this GO has children GO,
		if (isOpen)
		{
			// Recursively render each one
			for (int index{ 0 }; index < m_loadedScene->GetRoot()->GetChildCount(); ++index)
			{
				DrawItem(m_loadedScene->GetRoot()->GetChild(index), index);
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
	bool EditorHierarchy::DrawItem(CPTransform* item, int index)
	{
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

		//-------------------------Tree Node Inbetween------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
		ImGui::InvisibleButton("Spacer", ImVec2(ImGui::GetWindowContentRegionMax().x, 4.0f));
		ImGui::PopStyleVar();

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* objData = ImGui::AcceptDragDropPayload("HIERARCHY_OBJ")) {
				// If dragged to another parent
				if (m_draggedItem->GetParent() != item->GetParent())
				{
					m_draggedItem->SetParent(item->GetParent());
					m_draggedItem->GetParent()->ReorderChild(m_draggedItem->GetParent()->GetChildCount() - 1, index);
				}
				else
				{
					m_draggedItem->GetParent()->ReorderChild(m_draggedItemIndex, index);
				}
				ImGui::EndDragDropTarget();
			}
		}
		//-------------------------Tree Node Inbetween------------------------

		//-------------------------Tree Node Item Render-------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
		bool isOpen{ ImGui::TreeNodeEx(item->gameObj->GetName().c_str(), flags) };
		ImGui::PopStyleVar();

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) 
		{
			m_clickedItem = item;
			onNewObjectSelected.Invoke(item->gameObj);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* objData = ImGui::AcceptDragDropPayload("HIERARCHY_OBJ"))
			{
				m_draggedItem->SetParent(item);
				m_draggedItem = nullptr;
			}
		}
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("HIERARCHY_OBJ", &index, sizeof(index));
			m_draggedItem = item;
			m_draggedItemIndex = index;
			ImGui::EndDragDropSource();
		}

		// If this GO has children GO,
		if (isOpen)
		{
			// Recursively render each one
			for (int i{ 0 }; i < item->GetChildCount(); ++i)
			{
				DrawItem(item->GetChild(i), i);
			}
			ImGui::TreePop();
		}
		//-------------------------Tree Node Item Render-------------------------
		ImGui::PopID();

		return true;
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

	/*!***********************************************************************
	  \brief
	  Deletes the clicked GameObject
	*************************************************************************/
	void EditorHierarchy::DeleteSelectedObject()
	{
		m_clickedItem = nullptr;

		if (!InspectorGameObject::Instance()->IsGOInspected()) return;

		std::shared_ptr<RemoveObjectCommand> removeCommand = std::make_shared<RemoveObjectCommand>(InspectorGameObject::Instance()->GetInspectedGO());
		COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(removeCommand));

		onNewObjectSelected.Invoke(nullptr);
	}

	/*!***********************************************************************
	  \brief
	  Deselected but does not delete the clicked GameObject
	*************************************************************************/
	void EditorHierarchy::DeselectSelectedObject()
	{
		m_clickedItem = nullptr;
		onNewObjectSelected.Invoke(nullptr);
	}

	/*!***********************************************************************
	 \brief
	 Returns the current GameObject clicked, if any
	*************************************************************************/
	CPTransform* EditorHierarchy::GetClickedItem()
	{
		return m_clickedItem;
	}

	/*!***********************************************************************
	  \brief
	  Deletes the clicked GameObject
	*************************************************************************/
	void DeleteSelectedObject()
	{
		EDITORHIERACHY->DeleteSelectedObject();
	}

	/*!***********************************************************************
	  \brief
	  Checks if the gameobject deleted during gameplay is the same as the
	  one inspected.
	*************************************************************************/
	void CheckGameObjectDeleted(GameObject* deletedGO)
	{
		if (EDITORHIERACHY->GetClickedItem() && EDITORHIERACHY->GetClickedItem() == deletedGO->GetComponent<CPTransform>())
		{
			EDITORHIERACHY->DeselectSelectedObject();
		}
	}
}
