/*!************************************************************************
 \file				EditorSceneView.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "LitterBox/Renderer/Renderer.h"
#include "EditorSceneView.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Physics/ColliderManager.h"

extern unsigned int svtcb;

namespace LB
{
	EditorSceneView* EDITORSCENEVIEW = nullptr;
	float zoomStep = 1.5f, zoomCurrent = 1.f, zoomMin = 0.5f;

	void move_cam_up() {
		Renderer::GRAPHICS->update_cam(0.f, 20.f);
	}
	void move_cam_down() {
		Renderer::GRAPHICS->update_cam(0.f, -20.f);
	}
	void move_cam_left() {
		Renderer::GRAPHICS->update_cam(-20.f, 0.f);
	}
	void move_cam_right() {
		Renderer::GRAPHICS->update_cam(20.f, 0.f);
	}
	void zoom_cam_in() {
		zoomCurrent += zoomStep * TIME->GetDeltaTime();
		Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
	}
	void zoom_cam_out() {
		zoomCurrent -= zoomStep * TIME->GetDeltaTime();
		zoomCurrent = (zoomCurrent > zoomMin) ? zoomCurrent : zoomMin;
		Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
	}

	EditorSceneView::EditorSceneView(std::string layerName) : Layer(layerName)
	{
		if (!EDITORSCENEVIEW)
			EDITORSCENEVIEW = this;
		else
			DebuggerLogError("Editor Game View already exist!");

		if (INPUT) {
			INPUT->SubscribeToKey(move_cam_up, LB::KeyCode::KEY_G, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(move_cam_down, LB::KeyCode::KEY_B, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(move_cam_left, LB::KeyCode::KEY_V, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(move_cam_right, LB::KeyCode::KEY_N, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
		}
	}

	void EditorSceneView::UpdateLayer()
	{
		static bool set{ false };
		if (INPUT && !set) {
			INPUT->SubscribeToKey(move_cam_up, LB::KeyCode::KEY_G, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(move_cam_down, LB::KeyCode::KEY_B, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(move_cam_left, LB::KeyCode::KEY_V, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(move_cam_right, LB::KeyCode::KEY_N, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(zoom_cam_in, LB::KeyCode::KEY_X, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(zoom_cam_out, LB::KeyCode::KEY_C, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
		
			set = true;
		}

		ImGui::Begin(GetName().c_str());

		Vec2<float> mousePos{};

		ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)svtcb, wsize, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				mousePos.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
				mousePos.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

				const char* assetPath = (const char*)assetData->Data;
				DebuggerLog(assetPath);
				ASSETMANAGER->SpawnGameObject(assetPath, mousePos);
			}
		}

		// Get the object based on the world position of the mouse
		if (ImGui::IsItemHovered())
		{
			mousePos.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
			mousePos.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

			// Set the mouse position to the world position
			EDITOR->SetMousePos(mousePos);

			// If collided, select the component and then set mouse position elsewhere and set the selected game object
			if (EDITOR->GetMousePicker()->GetComponent<CPCollider>()->m_collided)
			{
				// Indicate that it collided
				//DEBUG->DrawCircle(EDITOR->GetMousePicker()->GetComponent<CPTransform>()->GetPosition(), 10.f, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});

				// Based on the number of objects that are collided with the mouse, pick the nearest object
				std::vector<GameObject*> objectsCollided = COLLIDERS->OverlapCircleGameObj(EDITOR->GetMousePicker()->GetComponent<CPTransform>()->GetPosition());
				// Distance checks
				float finalDist{100000.f};
				int index{};

				for (int i{}; i < objectsCollided.size(); ++i)
				{
					float objDistFromMouse = (EDITOR->GetMousePicker()->GetComponent<CPTransform>()->GetPosition() - objectsCollided[i]->GetComponent<CPTransform>()->GetPosition()).Length();
					if (objDistFromMouse < finalDist)
					{
						index = i;
					}
				}

				EDITOR->SetObjectPicked(objectsCollided[index]);
				// For future, do an additional check if that both object has a render component
				// 2 Scenarios - 1 has and 1 doesn't -> Take the one with the render component
				//			   - Both has -> Take the one that is rendered later
			}
		}

		ImGui::EndChild();

		ImGui::End();
	}
}