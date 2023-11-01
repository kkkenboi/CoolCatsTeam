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
#include "LitterBox/Utils/GameObjClicker.h"

extern unsigned int svtcb;

namespace LB
{
	EditorSceneView* EDITORSCENEVIEW = nullptr;
	float zoomStep = 1.5f, zoomCurrent = 1.f, zoomMin = 0.5f;

	void MoveCamUp() {
		Renderer::GRAPHICS->update_cam(0.f, 20.f);
	}
	void MoveCamDown() {
		Renderer::GRAPHICS->update_cam(0.f, -20.f);
	}
	void MoveCamLeft() {
		Renderer::GRAPHICS->update_cam(-20.f, 0.f);
	}
	void MoveCamRight() {
		Renderer::GRAPHICS->update_cam(20.f, 0.f);
	}
	void ZoomCamIn() {
		zoomCurrent += zoomStep * TIME->GetUnscaledDeltaTime();
		Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
	}
	void ZoomCamOut() {
		zoomCurrent -= zoomStep * TIME->GetUnscaledDeltaTime();
		zoomCurrent = (zoomCurrent > zoomMin) ? zoomCurrent : zoomMin;
		Renderer::GRAPHICS->fcam_zoom(zoomCurrent);
	}

	EditorSceneView::EditorSceneView(std::string layerName) : Layer(layerName)
	{
		if (!EDITORSCENEVIEW)
			EDITORSCENEVIEW = this;
		else
			DebuggerLogError("Editor Game View already exist!");
	}

	void EditorSceneView::Initialize()
	{
			INPUT->SubscribeToKey(MoveCamUp, LB::KeyCode::KEY_G, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(MoveCamDown, LB::KeyCode::KEY_B, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(MoveCamLeft, LB::KeyCode::KEY_V, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(MoveCamRight, LB::KeyCode::KEY_N, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);

			INPUT->SubscribeToKey(ZoomCamIn, LB::KeyCode::KEY_X, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);
			INPUT->SubscribeToKey(ZoomCamOut, LB::KeyCode::KEY_C, LB::KeyEvent::PRESSED, LB::KeyTriggerType::NONPAUSABLE);

			//INPUT->SubscribeToKey(onClick, LB::KeyCode::KEY_MOUSE_1, LB::KeyEvent::TRIGGERED, LB::KeyTriggerType::NONPAUSABLE);
	}

	void EditorSceneView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Renders the scene view as an image from the opengl buffer
		ImGui::BeginChild("GameRender");
		windowSize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)svtcb, windowSize, ImVec2(0, 1), ImVec2(1, 0));

		// If a prefab json file has been dropped onto the scene view
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("PREFAB"))
			{
				mousePosInWindow.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
				mousePosInWindow.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

				const char* assetPath = (const char*)assetData->Data;
				ASSETMANAGER->SpawnGameObject(assetPath, mousePosInWindow);
			}
		}

		// If the user has clicked on the scene view, check if they clicked on a GameObject
		if (ImGui::IsItemClicked())
		{
			mousePosInWindow.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
			mousePosInWindow.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

			EDITOR->SetObjectPicked(CheckMousePosGameObj(EDITOR->GetMousePicker()->GetComponent<CPTransform>()->GetPosition()));
		}
		ImGui::EndChild();

		ImGui::End();
	}
}