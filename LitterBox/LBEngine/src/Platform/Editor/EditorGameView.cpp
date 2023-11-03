/*!************************************************************************
 \file				EditorGameView.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This source file contains functions definitions for the game view layer of the
 Editor. This is to show how the game will look like during game mode, based on
 the player's perspective.

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

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorGameView class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorGameView::EditorGameView(std::string layerName) : Layer(layerName)
	{
		if (!EDITORGAMEVIEW)
			EDITORGAMEVIEW = this;
		else
			DebuggerLogError("Editor Game View already exist!");
	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorGameView layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorGameView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::BeginChild("GameRender");
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)(uintptr_t)textureColorbuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));

		// Render text
		//Renderer::GRAPHICS->render_msg("HELLO", 20.f, 20.f, 2.f, { .4f, .3f, 0.7f });

		m_MousePos.x = ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) / (ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x)) * WINDOWSSYSTEM->GetWidth();
		m_MousePos.y = (1.0f - (ImGui::GetMousePos().y - ImGui::GetItemRectMin().y) / (ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y)) * WINDOWSSYSTEM->GetHeight();

		ImGui::EndChild();

		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Get the current mouse position in the game view.
	  \return
	  The mouse position.
	*************************************************************************/
	Vec2<float>& EditorGameView::GetMousePos() {
		return m_MousePos;
	}

}
