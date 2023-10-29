/*!************************************************************************
 \file				EditorToolBar.cpp
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
#include "EditorToolBar.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	EditorToolBar::EditorToolBar(std::string layerName) : Layer(layerName) {}

	void EditorToolBar::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

		// Offset to center buttons
		float leftPadding{ (ImGui::GetWindowSize().x - m_buttonSpacing * 2.f - m_buttonSize.x * 3.f) * 0.5f };
		ImGui::Dummy({ leftPadding , m_buttonSize.y });
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, CORE->IsPlaying() ? m_buttonOnColor : m_buttonOffColor);
		if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex("PlayIcon"), m_buttonSize))
		{
			CORE->TogglePlaying();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine(0.f, m_buttonSpacing);

		ImGui::PushStyleColor(ImGuiCol_Button, TIME->IsPaused() ? m_buttonOnColor : m_buttonOffColor);
		if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex("PauseIcon"), m_buttonSize))
		{
			TIME->Pause(!TIME->IsPaused());
		}
		ImGui::PopStyleColor();
		
		ImGui::SameLine(0.f, m_buttonSpacing);

		ImGui::PushStyleColor(ImGuiCol_Button, m_buttonOffColor);
		if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex("StepIcon"), m_buttonSize))
		{
			DEBUG->StepPhysics();
		}
		ImGui::PopStyleColor();

		ImGui::End();
	}
}
