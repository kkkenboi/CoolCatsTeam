/*!************************************************************************
 \file				EditorAnimationEditor.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the EditorAniamtionEditor class and all its functionalities 
 for the aniamtion view layer of the Editor.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorAnimationEditor.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Animation/SpriteSheet.h" //Since it uses sprite sheet to animate

/*!***********************************************************************
\brief
Constructor of the Animation Editor
*************************************************************************/

namespace LB
{
	static float columnWidth = 250.0f;
	static float normalWidth = 150.f;
	static float smallWidth = 50.f;

	EditorAnimationEditor::EditorAnimationEditor(std::string layerName) : Layer(layerName)
	{

	}

	/*!***********************************************************************
	\brief
	 Updating the layers of the animator editor
	*************************************************************************/
	void EditorAnimationEditor::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		if (ImGui::Button("Save"))
		{
			Save();
		}

		if (m_stateLoaded)
		{
			ImGui::Text("Spritesheet");
			ImGui::SetNextItemWidth(columnWidth);
			if (ImGui::BeginCombo("##Spritesheet", (m_currentState.m_spriteSheet ? m_currentState.m_spriteSheet->GetName().c_str() : "None") ))
			{
				for (auto& [str, sSheet] : ASSETMANAGER->SpriteSheets)
				{
					std::filesystem::path tempPath{ str };
					if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
					{
						m_currentState.m_spriteSheet = &sSheet;
						break;
					}
				}

				ImGui::EndCombo();
			}

			//display details of each tile here
			if (m_currentState.m_spriteSheet)
			{
				if (ImGui::BeginTable("SlicedSpriteSheet", m_currentState.m_spriteSheet->m_col))
				{
					//Creating a table to place the sprites evenly by its row and cols
					for (int r = { 0 }; r < m_currentState.m_spriteSheet->m_row; ++r) //go thru rows
					{
						ImGui::TableNextRow(); //go next row
						for (int c = 0; c < m_currentState.m_spriteSheet->m_col; ++c) //go thru cols
						{
							ImGui::TableSetColumnIndex(c);

							int tileNum = (c + r * m_currentState.m_spriteSheet->m_col) + 1;
							ImGui::PushID(tileNum);
							ImGui::Text("Frame %i", tileNum);
							if (ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex(m_currentState.m_spriteSheet->GetPNGRef()), ImVec2{ smallWidth, smallWidth }
								, ImVec2{ (*m_currentState.m_spriteSheet)[c + r * m_currentState.m_spriteSheet->m_row].m_min.x, (*m_currentState.m_spriteSheet)[c + r * m_currentState.m_spriteSheet->m_row].m_max.y }
								, ImVec2{ (*m_currentState.m_spriteSheet)[c + r * m_currentState.m_spriteSheet->m_row].m_max.x, (*m_currentState.m_spriteSheet)[c + r * m_currentState.m_spriteSheet->m_row].m_min.y }))
							{

							}
							ImGui::PopID();
						}
					}
					ImGui::EndTable();
				}
			}

			ImGui::Text("Frame");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::InputInt("##FrameIndex", &m_currentKeyFrame.m_frame))
			{

			}
			ImGui::SameLine();
			ImGui::Text("Time");
			ImGui::SameLine();
			
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::DragFloat("##FrameTime", &m_currentKeyFrame.m_time))
			{

			}

			ImGui::Dummy(ImVec2(0.0f, 35.0f));

			if (ImGui::Button("Add KeyFrame"))
			{
				m_currentState.AddFrame(m_currentKeyFrame);
			}

			ImGui::Text("Frames");
			for (int index{ 0 }; index < m_currentState.GetFrameCount(); ++index)
			{
				ImGui::Text("Frame");
				ImGui::SameLine();

				ImGui::PushID(index);
				m_tempKeyFrame.m_frame = m_currentState[index].m_frame;
				
				ImGui::SetNextItemWidth(normalWidth);
				if (ImGui::InputInt("##FrameIndex", &m_tempKeyFrame.m_frame))
				{
					m_currentState[index].m_frame = m_tempKeyFrame.m_frame;
				}
				ImGui::SameLine();

				ImGui::Text("Time");
				ImGui::SameLine();
				m_tempKeyFrame.m_time = m_currentState[index].m_time;
				
				ImGui::SetNextItemWidth(normalWidth);
				if (ImGui::DragFloat("##FrameTime", &m_tempKeyFrame.m_time))
				{
					m_currentState[index].m_time = m_tempKeyFrame.m_time;
				}
				ImGui::PopID();
			}
		}
		else if (m_controllerLoaded)
		{

		}

		ImGui::End();
	}

	void EditorAnimationEditor::Save()
	{
		if (m_stateLoaded)
		{
			JSONSerializer::SerializeToFile(m_currentState.GetName(), m_currentState);
		}
		else if (m_controllerLoaded)
		{
			JSONSerializer::SerializeToFile(m_currentController.GetName(), m_currentController);
		}
	}

	void EditorAnimationEditor::LoadState(std::string const& name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_currentState);
		m_stateLoaded = true;
		m_controllerLoaded = false;
	}

	void EditorAnimationEditor::LoadController(std::string const& name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_currentController);
		m_controllerLoaded = true;
		m_stateLoaded = false;
	}
}
