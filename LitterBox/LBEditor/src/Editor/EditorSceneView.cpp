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
#include "Commands/GameObjectCommands.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <limits>

extern unsigned int svtcb;
extern Renderer::RenderSystem* Renderer::GRAPHICS;

namespace LB
{
	EditorSceneView* EDITORSCENEVIEW = nullptr;

	// Manipulate Variables
	ImVec2 vpMinMax[2];
	float outOfView[16]
	{
		std::numeric_limits<float>::max(), 0.f, 0.f, 0.f,
		std::numeric_limits<float>::max(), 1.f, 0.f, 0.f,
		std::numeric_limits<float>::max(), 0.f, 1.f, 0.f,
		std::numeric_limits<float>::max(), 0.f, 0.f, 1.f
	};

	float zoomStep = 1.5f, zoomCurrent = 1.f, zoomMin = 0.25f;

	/*!***********************************************************************
	  \brief
	  Moves the scene view camera upwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamUp() {
		EDITORSCENEVIEW->MoveCamUp();
	}
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera downwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamDown() {
		EDITORSCENEVIEW->MoveCamDown();
	}
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera leftwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamLeft() {
		EDITORSCENEVIEW->MoveCamLeft();
	}
	/*!***********************************************************************
	  \brief
	  Moves the scene view camera rightwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void MoveCamRight() {
		EDITORSCENEVIEW->MoveCamRight();
	}
	/*!***********************************************************************
	  \brief
	  Zooms the scene view camera inwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void ZoomCamIn() {
		EDITORSCENEVIEW->ZoomCamIn();
	}
	/*!***********************************************************************
	  \brief
	  Zooms the scene view camera outwards. Function is passed as an event.
	  \return
	  Nothing.
	*************************************************************************/
	void ZoomCamOut() {
		EDITORSCENEVIEW->ZoomCamOut();
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
			INPUT->SubscribeToKey(LB::MoveCamUp, LB::KeyCode::KEY_UP, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(LB::MoveCamDown, LB::KeyCode::KEY_DOWN, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(LB::MoveCamLeft, LB::KeyCode::KEY_LEFT, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(LB::MoveCamRight, LB::KeyCode::KEY_RIGHT, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);

			INPUT->SubscribeToKey(LB::ZoomCamIn, LB::KeyCode::KEY_X, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(LB::ZoomCamOut, LB::KeyCode::KEY_C, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);

			// Add the mouse picker object and point to its position (for easy updating)
			m_mousePicker = FACTORY->SpawnGameObject({C_CPCollider}, GOSpawnType::FREE_FLOATING);
			m_mousePicker->GetComponent<CPTransform>()->SetScale({ 0.1f,0.1f });

			//INPUT->SubscribeToKey(onClick, LB::KeyCode::KEY_MOUSE_1, LB::KeyEvent::TRIGGERED, LB::KeyTriggerType::NONPAUSABLE);

			//InspectorGameObject::Instance()->SetGizmosMode(ImGuizmo::WORLD);
			//InspectorGameObject::Instance()->SetGizmosOperation(ImGuizmo::UNIVERSAL);
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

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::BeginFrame();

		auto vpMin = ImGui::GetWindowContentRegionMin();
		auto vpMax= ImGui::GetWindowContentRegionMax();
		auto vpOffset = ImGui::GetWindowPos();

		vpMinMax[0] = { vpMin.x + vpOffset.x, vpMin.y + vpOffset.y };
		vpMinMax[1] = { vpMax.x + vpOffset.x, vpMax.y + vpOffset.y };

		// Renders the scene view as an image from the opengl buffer
		ImGui::BeginChild("GameRender");
		m_windowSize = ImGui::GetWindowSize();
		uint64_t warning_remover = (uint64_t)Renderer::GRAPHICS->get_scene_view();

		if(warning_remover != static_cast<unsigned int>(-1))// && !VideoPlayerSystem::Instance()->get_frame())
			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(Renderer::GRAPHICS->get_scene_view())), m_windowSize, ImVec2(0, 1), ImVec2(1, 0));

		//WAS USED FOR TESTING REOMVE LATER
		/*if(VideoPlayerSystem::Instance()->get_frame())
			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(VideoPlayerSystem::Instance()->get_frame())), m_windowSize);*/
		
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			m_oldMousePosInWorld = m_mousePosInWorld;
		}
		if (ImGui::GetIO().MouseWheel != 0.0f) {
			ImGui::GetIO().MouseWheel > 0.0f ? ZoomCamIn() : ZoomCamOut();
		}

		m_mousePosInWorld.x = (((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth()) / zoomCurrent + Renderer::GRAPHICS->get_cam()->get_cam_pos().x;
		m_mousePosInWorld.y = ((1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight()) / zoomCurrent + Renderer::GRAPHICS->get_cam()->get_cam_pos().y;

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			Vec2<float> delta = m_mousePosInWorld - m_oldMousePosInWorld;
			delta *= TIME->GetUnscaledDeltaTime() * m_camDragSpeed;
			MoveCam(delta);
		}

		// If the user is using the scene view
		usingSceneView = ImGui::IsWindowHovered();

		// If a prefab json file has been dropped onto the scene view
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				const char* assetPath = (const char*)assetData->Data;

				//Spawn GO Command
				std::shared_ptr<SpawnObjectCommand> spawnCommand = std::make_shared<SpawnObjectCommand>(assetPath, m_mousePosInWorld);
				COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(spawnCommand));

				// ASSETMANAGER->SpawnGameObject(assetPath, m_mousePosInWorld);
			}
		}
		// If the user has clicked on the scene view, check if they clicked on a GameObject
		if (ImGui::IsItemClicked() && !ImGuizmo::IsOver())
		{
			SetObjectPicked(CheckMousePosGameObj(m_mousePosInWorld));
		}
		// ----------------------------------------------
		if (InspectorGameObject::Instance()->GetInspectedGO() && !InspectorGameObject::Instance()->isPrefab) // TODO: Less magic prefab editting implementation bools
		{
			auto trans = InspectorGameObject::Instance()->GetInspectedGO()->GetComponent<CPTransform>()->GetPosition();
			auto rot = InspectorGameObject::Instance()->GetInspectedGO()->GetComponent<CPTransform>()->GetRotation();
			auto scale = InspectorGameObject::Instance()->GetInspectedGO()->GetComponent<CPTransform>()->GetScale();
			
			glm::mat4 transform{ 1.0f };

			transform = glm::translate(glm::mat4{ 1.0f }, glm::vec3(trans.x, trans.y, 0.0f))
						* glm::rotate(glm::mat4{ 1.0f }, glm::radians(rot), glm::vec3{ 0.0f, 0.0f, 1.0f })
						* glm::scale(glm::mat4{ 1.0f }, glm::vec3{ scale.x, scale.y, 1.0f });

			ImGuizmo::SetRect(vpMinMax[0].x, vpMinMax[0].y, vpMinMax[1].x - vpMinMax[0].x, vpMinMax[1].y - vpMinMax[0].y);
			ImGuizmo::SetDrawlist();

			// At all times, when a GizmosOperation is set, it means that there will be snapping applied onto the object
			// If the GizmosOperation is set to Universal, there will be no snapping.
			// Object can still be moved/edited freely through the EditorInspector interface.
			//switch (InspectorGameObject::Instance()->GetGizmosOperation())
			//{
			//case ImGuizmo::TRANSLATE:
			//	ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam()->get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam()->editor_ortho),
			//		InspectorGameObject::Instance()->GetGizmosOperation(), InspectorGameObject::Instance()->GetGizmosMode(), glm::value_ptr(transform), NULL,
			//		&InspectorGameObject::Instance()->GetSnapTranslate());
			//	break;
			//case ImGuizmo::ROTATE:
			//	ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam()->get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam()->editor_ortho),
			//		InspectorGameObject::Instance()->GetGizmosOperation(), InspectorGameObject::Instance()->GetGizmosMode(), glm::value_ptr(transform), NULL,
			//		&InspectorGameObject::Instance()->GetSnapRotate());
			//	break;
			//case ImGuizmo::SCALE:
			//	ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam()->get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam()->editor_ortho),
			//		InspectorGameObject::Instance()->GetGizmosOperation(), InspectorGameObject::Instance()->GetGizmosMode(), glm::value_ptr(transform), NULL,
			//		&InspectorGameObject::Instance()->GetSnapScale());
			//	break;
			//case ImGuizmo::UNIVERSAL: // No snapping is applied
			//	
			//	break;
			//}

			ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam()->get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam()->editor_ortho),
				ImGuizmo::TRANSLATE | ImGuizmo::SCALE | ImGuizmo::ROTATE_Z, ImGuizmo::LOCAL, glm::value_ptr(transform), NULL, NULL);

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
					std::shared_ptr<MoveCommand> moveCommand = std::make_shared<MoveCommand>(InspectorGameObject::Instance()->GetInspectedGO()->GetComponent<CPTransform>(), finalTrans);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(moveCommand));
				}
				if (fabs(finalScale.x - scale.x) > EPSILON_F || fabs(finalScale.y - scale.y) > EPSILON_F)
				{
					std::shared_ptr<ScaleCommand> scaleCommand = std::make_shared<ScaleCommand>(InspectorGameObject::Instance()->GetInspectedGO()->GetComponent<CPTransform>(), finalScale);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(scaleCommand));
				}
				if (fabs(finalRot - rot) > EPSILON_F)
				{
					std::shared_ptr<RotateCommand> rotateCommand = std::make_shared<RotateCommand>(InspectorGameObject::Instance()->GetInspectedGO()->GetComponent<CPTransform>(), finalRot);
					COMMAND->AddCommand(std::dynamic_pointer_cast<ICommand>(rotateCommand));
				}
				/*EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetPosition(finalTrans);
				EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetRotation(finalRot);
				EDITORINSPECTOR->GetInspectedGO()->GetComponent<CPTransform>()->SetScale(finalScale);*/
			}
		}
		if (!InspectorGameObject::Instance()->GetInspectedGO())
		{
			ImGuizmo::Manipulate(glm::value_ptr(Renderer::GRAPHICS->get_cam()->get_free_cam()), glm::value_ptr(Renderer::GRAPHICS->get_cam()->editor_ortho),
				InspectorGameObject::Instance()->GetGizmosOperation(), InspectorGameObject::Instance()->GetGizmosMode(), outOfView, NULL,
				&InspectorGameObject::Instance()->GetSnapTranslate());
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
		InspectorGameObject::Instance()->UpdateInspectedGO(obj);
		EDITORHIERACHY->UpdateClickedItem(obj ? obj->GetComponent<CPTransform>() : nullptr);
	}


	/*!***********************************************************************
	  \brief
	  Moves the scene view camera upwards.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::MoveCamUp(float dist)
	{
		if (usingSceneView && INPUT->IsKeyPressed(m_moveCamKey))
		{
			Renderer::GRAPHICS->update_cam(0.f, dist);
		}
	}

	/*!***********************************************************************
	  \brief
	  Moves the scene view camera downwards.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::MoveCamDown(float dist)
	{
		if (usingSceneView && INPUT->IsKeyPressed(m_moveCamKey))
		{
			Renderer::GRAPHICS->update_cam(0.f, -dist);
		}
	}

	/*!***********************************************************************
	  \brief
	  Moves the scene view camera leftwards.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::MoveCamLeft(float dist)
	{
		if (usingSceneView && INPUT->IsKeyPressed(m_moveCamKey))
		{
			Renderer::GRAPHICS->update_cam(-dist, 0.f);
		}
	}


	/*!***********************************************************************
	  \brief
	  Moves the scene view camera rightwards.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::MoveCamRight(float dist)
	{
		if (usingSceneView && INPUT->IsKeyPressed(m_moveCamKey))
		{
			Renderer::GRAPHICS->update_cam(dist, 0.f);
		}
	}

	/*!***********************************************************************
	  \brief
	  Moves the scene view camera.
	*************************************************************************/
	void EditorSceneView::MoveCam(Vec2<float> const& dist)
	{
		if (usingSceneView)
		{
			Renderer::GRAPHICS->update_cam(dist.x, dist.y);
		}
	}


	/*!***********************************************************************
	  \brief
	  Zooms the scene view camera inwards.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::ZoomCamIn()
	{
		if (usingSceneView)
		{
			zoomCurrent += zoomStep * static_cast<float>(TIME->GetUnscaledDeltaTime());
			Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
		}
	}


	/*!***********************************************************************
	  \brief
	  Zooms the scene view camera outwards.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorSceneView::ZoomCamOut()
	{
		if (usingSceneView)
		{
			zoomCurrent -= zoomStep * static_cast<float>(TIME->GetUnscaledDeltaTime());
			zoomCurrent = (zoomCurrent > zoomMin) ? zoomCurrent : zoomMin;
			Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
		}
	}
}