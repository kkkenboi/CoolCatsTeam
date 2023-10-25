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

	Vec2<float> const& EditorGameView::GetMousePos()
	{
		return mousePosInWindow;
	}

	void EditorGameView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 mousePos = ImGui::GetMousePos();
		ImVec2 windowMax = ImGui::GetWindowContentRegionMax();
		mousePosInWindow.x = mousePos.x - windowPos.x;
		mousePosInWindow.y = mousePos.y - windowPos.y;
		/*mousePosInWindow.x = mousePos.x;
		mousePosInWindow.y = mousePos.y;*/
		//DebuggerLogFormat("Mouse pos : %f,%f", GetMousePos().x, GetMousePos().y);
		//DebuggerLogFormat("Window pos : %f,%f", windowPos.x, windowPos.y);
		DebuggerLogFormat("Window max : %f,%f", windowMax.x, windowMax.y);
		ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* assetData = ImGui::AcceptDragDropPayload("ASSET BROWSER"))
			{
				const char* assetPath = (const char*)assetData->Data;
				DebuggerLog(assetPath);
				ASSETMANAGER->SpawnGameObject(assetPath, Vec2<float>{GetMousePos().x,GetMousePos().y});
			}
		}


		ImGui::EndChild();

		ImGui::End();
	}
}
