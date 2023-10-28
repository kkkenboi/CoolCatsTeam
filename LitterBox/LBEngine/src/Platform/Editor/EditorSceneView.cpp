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

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 mousePos = ImGui::GetMousePos();
		mousePosInWindow.x = mousePos.x - windowPos.x;
		mousePosInWindow.y = mousePos.y - windowPos.y;

		ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)svtcb, wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();

		ImGui::End();
	}
}