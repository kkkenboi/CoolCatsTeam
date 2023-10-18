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

extern unsigned int textureColorbuffer;

namespace LB
{

	EditorGameView::EditorGameView(std::string layerName) : Layer(layerName)
	{

	}

	void EditorGameView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();

		ImGui::End();
	}
}
