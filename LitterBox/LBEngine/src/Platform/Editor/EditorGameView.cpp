/*!************************************************************************
 \file				EditorGameView.cpp
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorGameView.h"
#include "LitterBox/Renderer/Renderer.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "Platform/Windows/Windows.h"

extern unsigned int textureColorbuffer;

namespace LB
{
	EditorGameView* EDITORGAMEVIEW = nullptr;

	EditorGameView::EditorGameView(std::string layerName) : Layer(layerName)
	{
		if (!EDITORGAMEVIEW)
			EDITORGAMEVIEW = this;
		else
			DebuggerLogError("Editor Game View already exist!");
	}

	void EditorGameView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));

		Vec2<float> mousePos{};

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("ASSET BROWSER"))
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

			// Check through the collider components
			if (EDITOR->GetMousePicker()->GetComponent<CPCollider>()->m_collided)
			{
				DEBUG->DrawCircle(EDITOR->GetMousePicker()->GetComponent<CPTransform>()->GetPosition(), 10.f, Vec4<float>{0.f, 0.f, 0.5f, 1.0f});
				COLLIDERS->OverlapCircle(EDITOR->GetMousePicker()->GetComponent<CPTransform>()->GetPosition(), 200.f);
			}
			// If collided, select the component and then set mouse position elsewhere and set the selected game object

		}



		ImGui::EndChild();

		ImGui::End();
	}
}
