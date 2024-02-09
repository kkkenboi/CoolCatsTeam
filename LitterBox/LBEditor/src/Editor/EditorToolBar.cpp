/*!************************************************************************
 \file				EditorToolBar.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file contains functions definitions for the toolbar layer of the
 Editor. This is to allow the programmer to play, stop and step in frame by frame.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorToolBar.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorToolBar class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorToolBar::EditorToolBar(std::string layerName) : Layer(layerName) {}

	/*!***********************************************************************
	  \brief
	  Updates the EditorToolBar layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorToolBar::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

		SpriteSheet& spriteSheet = ASSETMANAGER->GetSpriteSheet("EditorSpriteSheet");

		// Offset to center buttons
		float leftPadding{ (ImGui::GetWindowSize().x - m_buttonSpacing * 2.f - m_buttonSize.x * 3.f) * 0.5f };
		ImGui::Dummy({ leftPadding , m_buttonSize.y });
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, CORE->IsPlaying() ? m_buttonOnColor : m_buttonOffColor);
		if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex(spriteSheet.GetPNGRef()), m_buttonSize
			, ImVec2{ spriteSheet[0].m_min.x, spriteSheet[0].m_max.y }
			, ImVec2{ spriteSheet[0].m_max.x, spriteSheet[0].m_min.y }))
		{
			CORE->TogglePlaying();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine(0.f, m_buttonSpacing);

		ImGui::PushStyleColor(ImGuiCol_Button, TIME->IsPaused() ? m_buttonOnColor : m_buttonOffColor);
		if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex(spriteSheet.GetPNGRef()), m_buttonSize
			, ImVec2{ spriteSheet[1].m_min.x, spriteSheet[1].m_max.y }
			, ImVec2{ spriteSheet[1].m_max.x, spriteSheet[1].m_min.y }))
		{
			TIME->Pause(!TIME->IsPaused());
		}
		ImGui::PopStyleColor();
		
		ImGui::SameLine(0.f, m_buttonSpacing);

		ImGui::PushStyleColor(ImGuiCol_Button, m_buttonOffColor);
		if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex(spriteSheet.GetPNGRef()), m_buttonSize
			, ImVec2{ spriteSheet[2].m_min.x, spriteSheet[2].m_max.y }
			, ImVec2{ spriteSheet[2].m_max.x, spriteSheet[2].m_min.y }))
		{
			DEBUG->StepPhysics();
		}
		ImGui::PopStyleColor();

		ImGui::End();
	}
}
