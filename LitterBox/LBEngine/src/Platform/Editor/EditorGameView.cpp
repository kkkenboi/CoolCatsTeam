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
#include "Platform/Windows/Windows.h"

extern unsigned int textureColorbuffer;
extern Renderer::RenderSystem* Renderer::GRAPHICS;

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

		// Render text
		//Renderer::GRAPHICS->render_msg("HELLO", 20.f, 20.f, 2.f, { .4f, .3f, 0.7f });

		ImGui::EndChild();

		ImGui::End();
	}

}
