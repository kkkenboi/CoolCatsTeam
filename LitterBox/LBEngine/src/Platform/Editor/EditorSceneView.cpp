/*!************************************************************************
 \file				EditorSceneView.cpp
 \author(s)			Ang Jiawei Jarrett, Kenji Brannon Chong
 \par DP email(s):	a.jiaweijarrett@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file contains functions definitions for the scene view layer of the
 Editor. This is to showcase the scene view, apart from the game view where it
 can be interacted with and moved around.


 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Utils/GameObjClicker.h"

#include "EditorSceneView.h"
#include "EditorHierarchy.h"
#include "EditorInspector.h"

extern Renderer::RenderSystem* Renderer::GRAPHICS;

namespace LB
{
	EditorSceneView* EDITORSCENEVIEW = nullptr;

	float zoomStep = 1.5f, zoomCurrent = 1.f, zoomMin = 0.5f;
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera upwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamUp() {
		Renderer::GRAPHICS->update_cam(0.f, 20.f);
	}
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera downwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamDown() {
		Renderer::GRAPHICS->update_cam(0.f, -20.f);
	}
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera leftwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamLeft() {
		Renderer::GRAPHICS->update_cam(-20.f, 0.f);
	}
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera rightwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamRight() {
		Renderer::GRAPHICS->update_cam(20.f, 0.f);
	}
	/*!***********************************************************************
	  \brief
	  Zooms the scene view camera inwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void ZoomCamIn() {
		zoomCurrent += zoomStep * static_cast<float>(TIME->GetUnscaledDeltaTime());
		Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
	}
	/*!***********************************************************************
	  \brief
	  Zooms the scene view camera outwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void ZoomCamOut() {
		zoomCurrent -= zoomStep * static_cast<float>(TIME->GetUnscaledDeltaTime());
		zoomCurrent = (zoomCurrent > zoomMin) ? zoomCurrent : zoomMin;
		Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
	}

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorSceneView class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorSceneView::EditorSceneView(std::string layerName) : Layer(layerName)
	{
		if (!EDITORSCENEVIEW)
			EDITORSCENEVIEW = this;
		else
			DebuggerLogError("Editor Game View already exist!");
	}

	/*!***********************************************************************
	  \brief
	  Initializes the EditorSceneView layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::Initialize()
	{
			INPUT->SubscribeToKey(MoveCamUp, LB::KeyCode::KEY_G, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(MoveCamDown, LB::KeyCode::KEY_B, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(MoveCamLeft, LB::KeyCode::KEY_V, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(MoveCamRight, LB::KeyCode::KEY_N, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);

			INPUT->SubscribeToKey(ZoomCamIn, LB::KeyCode::KEY_X, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(ZoomCamOut, LB::KeyCode::KEY_C, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);

			// Add the mouse picker object and point to its position (for easy updating)
			m_mousePicker = FACTORY->SpawnGameObject({C_CPCollider}, GOSpawnType::FREE_FLOATING);
			m_mousePicker->GetComponent<CPTransform>()->SetScale({ 0.1f,0.1f });

			//INPUT->SubscribeToKey(onClick, LB::KeyCode::KEY_MOUSE_1, LB::KeyEvent::TRIGGERED, LB::KeyTriggerType::NONPAUSABLE);
	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorSceneView layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Renders the scene view as an image from the opengl buffer
		ImGui::BeginChild("GameRender");
		m_windowSize = ImGui::GetWindowSize();
		ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(Renderer::GRAPHICS->get_scene_view())), m_windowSize, ImVec2(0, 1), ImVec2(1, 0));

		// If a prefab json file has been dropped onto the scene view
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				m_mousePosInWorld.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
				m_mousePosInWorld.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

				const char* assetPath = (const char*)assetData->Data;
				ASSETMANAGER->SpawnGameObject(assetPath, m_mousePosInWorld);
			}
		}

		// If the user has clicked on the scene view, check if they clicked on a GameObject
		if (ImGui::IsItemClicked())
		{
			m_mousePosInWorld.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
			m_mousePosInWorld.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

			SetObjectPicked(CheckMousePosGameObj(m_mousePosInWorld));
		}
		// If the user is dragging the mouse while a GameObject is selected, have the GameObject follow the cursor
		if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(0) && EDITORINSPECTOR->IsGOInspected())
		{
			m_mousePosInWorld.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
			m_mousePosInWorld.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

			EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetPosition(m_mousePosInWorld);
		}

		ImGui::EndChild();

		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Set the selected object in the scene.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::SetObjectPicked(GameObject* obj)
	{
		EDITORINSPECTOR->UpdateInspectedGO(obj);
		EDITORHIERACHY->UpdateClickedItem(obj ? obj->GetComponent<CPTransform>() : nullptr);
	}
}