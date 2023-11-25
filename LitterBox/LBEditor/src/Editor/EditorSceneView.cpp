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

#include "Utils/CommandManager.h"
#include "Commands/TransformCommands.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include <memory>

extern unsigned int svtcb;
extern Renderer::RenderSystem* Renderer::GRAPHICS;

namespace LB
{
	EditorSceneView* EDITORSCENEVIEW = nullptr;

	// Manipulate Variables
	ImVec2 vpMinMax[2];

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

			EDITORINSPECTOR->SetGizmosMode(ImGuizmo::WORLD);
			EDITORINSPECTOR->SetGizmosOperation(ImGuizmo::UNIVERSAL);
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
		ImGuizmo::BeginFrame();

		ImGuizmo::SetOrthographic(true);

		auto vpMin = ImGui::GetWindowContentRegionMin();
		auto vpMax= ImGui::GetWindowContentRegionMax();
		auto vpOffset = ImGui::GetWindowPos();

		vpMinMax[0] = { vpMin.x + vpOffset.x, vpMin.y + vpOffset.y };
		vpMinMax[1] = { vpMax.x + vpOffset.x, vpMax.y + vpOffset.y };

		// Renders the scene view as an image from the opengl buffer
		ImGui::BeginChild("GameRender");
		m_windowSize = ImGui::GetWindowSize();
		uint64_t warning_remover = (uint64_t)Renderer::GRAPHICS->get_scene_view();
		if(warning_remover != static_cast<unsigned int>(-1))
			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(Renderer::GRAPHICS->get_scene_view())), m_windowSize, ImVec2(0, 1), ImVec2(1, 0));

		// If a prefab json file has been dropped onto the scene view
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				m_mousePosInWorld.x = (((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth()) / zoomCurrent + Renderer::GRAPHICS->get_cam().get_cam_pos().x;
				m_mousePosInWorld.y = ((1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight()) / zoomCurrent + Renderer::GRAPHICS->get_cam().get_cam_pos().y;

				const char* assetPath = (const char*)assetData->Data;
				ASSETMANAGER->SpawnGameObject(assetPath, m_mousePosInWorld);
			}
		}

		// If the user has clicked on the scene view, check if they clicked on a GameObject
		if (ImGui::IsItemClicked() && !ImGuizmo::IsOver())
		{
			m_mousePosInWorld.x = (((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth()) / zoomCurrent + Renderer::GRAPHICS->get_cam().get_cam_pos().x;
			m_mousePosInWorld.y = ((1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight()) / zoomCurrent + Renderer::GRAPHICS->get_cam().get_cam_pos().y;

			SetObjectPicked(CheckMousePosGameObj(m_mousePosInWorld));
		}
		// If the user is dragging the mouse while a GameObject is selected, have the GameObject follow the cursor
		//if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(0) && EDITORINSPECTOR->IsGOInspected() && !ImGuizmo::IsOver())
		//{
		//	m_mousePosInWorld.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
		//	m_mousePosInWorld.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

		//	EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetPosition(m_mousePosInWorld);
		//}
		// Set the different ImGuizmo operation modes here

		// ----------------------------------------------
		if (EDITORINSPECTOR->GetInspectedGO() && !EDITORINSPECTOR->isPrefab) // TODO: Less magic prefab editting implementation bools
		{
			auto trans = EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->GetPosition();
			auto rot = EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->GetRotation();
			auto scale = EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->GetScale();
			
			glm::mat4 transform{ 1.0f };

			transform = glm::translate(glm::mat4{ 1.0f }, glm::vec3(trans.x, trans.y, 0.0f))
						* glm::rotate(glm::mat4{ 1.0f }, glm::radians(rot), glm::vec3{ 0.0f, 0.0f, 1.0f })
						* glm::scale(glm::mat4{ 1.0f }, glm::vec3{ scale.x, scale.y, 1.0f });

			ImGuizmo::SetRect(vpMinMax[0].x, vpMinMax[0].y, vpMinMax[1].x - vpMinMax[0].x, vpMinMax[1].y - vpMinMax[0].y);
			ImGuizmo::SetDrawlist();

			// At all times, when a GizmosOperation is set, it means that there will be snapping applied onto the object
			// If the GizmosOperation is set to Universal, there will be no snapping.
			// Object can still be moved/edited freely through the EditorInspector interface.
			switch (EDITORINSPECTOR->GetGizmosOperation())
			{
			case ImGuizmo::TRANSLATE:
				ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam().get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam().editor_ortho),
					EDITORINSPECTOR->GetGizmosOperation(), EDITORINSPECTOR->GetGizmosMode(), glm::value_ptr(transform), NULL,
					&EDITORINSPECTOR->GetSnapTranslate());
				break;
			case ImGuizmo::ROTATE:
				ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam().get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam().editor_ortho),
					EDITORINSPECTOR->GetGizmosOperation(), EDITORINSPECTOR->GetGizmosMode(), glm::value_ptr(transform), NULL,
					&EDITORINSPECTOR->GetSnapRotate());
				break;
			case ImGuizmo::SCALE:
				ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam().get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam().editor_ortho),
					EDITORINSPECTOR->GetGizmosOperation(), EDITORINSPECTOR->GetGizmosMode(), glm::value_ptr(transform), NULL,
					&EDITORINSPECTOR->GetSnapScale());
				break;
			case ImGuizmo::UNIVERSAL: // No snapping is applied
				ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam().get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam().editor_ortho),
					EDITORINSPECTOR->GetGizmosOperation(), EDITORINSPECTOR->GetGizmosMode(), glm::value_ptr(transform), NULL,
					NULL);
				break;
			}
			// When using the gizmos, set the values after Manipulate is called on the object
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 decompTrans, decompRot, decompScale;

				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
													  glm::value_ptr(decompTrans), glm::value_ptr(decompRot), glm::value_ptr(decompScale));

				Vec2<float> finalTrans = { decompTrans.x, decompTrans.y };
				float finalRot = { decompRot.z };
				Vec2<float> finalScale = { decompScale.x, decompScale.y };
				// Set the new values to translate, rotate and scale 
				if (!(finalTrans == trans))
				{
					std::shared_ptr<MoveCommand> moveCommand = std::make_shared<MoveCommand>(EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>(), finalTrans);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(moveCommand));
				}
				if (fabs(finalScale.x - scale.x) > EPSILON_F || fabs(finalScale.y - scale.y) > EPSILON_F)
				{
					std::shared_ptr<ScaleCommand> scaleCommand = std::make_shared<ScaleCommand>(EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>(), finalScale);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(scaleCommand));
				}
				if (fabs(finalRot - rot) > EPSILON_F)
				{
					std::shared_ptr<RotateCommand> rotateCommand = std::make_shared<RotateCommand>(EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>(), finalRot);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(rotateCommand));
				}
				/*EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetPosition(finalTrans);
				EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetRotation(finalRot);
				EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetScale(finalScale);*/
			}
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