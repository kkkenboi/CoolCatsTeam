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
	static float smallWidth = 100.f;

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

		//----------------------------------------------ANIMATION STATE EDITING----------------------------------------------
		if (m_stateLoaded)
		{
			//----------------------------------------------STATE NAME----------------------------------------------
			ImGui::Text("%-17s", "Name");
			ImGui::SameLine();
			ImGui::Text(m_currentState.GetName().c_str());

			//----------------------------------------------SPRITESHEET SELECTION----------------------------------------------
			ImGui::Text("%-17s", "Spritesheet");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(columnWidth);
			if (ImGui::BeginCombo("##Spritesheet", (m_spriteSheet ? m_spriteSheet->GetName().c_str() : "None") ))
			{
				for (auto& [str, sSheet] : ASSETMANAGER->SpriteSheets)
				{
					std::filesystem::path tempPath{ str };
					if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
					{
						m_spriteSheet = &sSheet;
						m_currentState.SetSpriteSheetName(sSheet.GetName());
						break;
					}
				}

				ImGui::EndCombo();
			}

			//display details of each tile here
			if (m_spriteSheet)
			{
				if (ImGui::BeginTable("SlicedSpriteSheet", m_spriteSheet->m_col, ImGuiTableFlags_SizingFixedFit))
				{
					//Creating a table to place the sprites evenly by its row and cols
					for (int r{ 0 }; r < m_spriteSheet->m_row; ++r) //go thru rows
					{
						ImGui::TableNextRow(); //go next row
						for (int c{ 0 }; c < m_spriteSheet->m_col; ++c) //go thru cols
						{
							ImGui::TableSetColumnIndex(c);

							int tileNum = (c + r * m_spriteSheet->m_col);
							
							ImGui::PushID(tileNum);
							ImGui::Text("Frame %i", tileNum);
							if (ImGui::ImageButton((ImTextureID)(uint64_t)ASSETMANAGER->GetTextureIndex(m_spriteSheet->GetPNGRef()), ImVec2{ smallWidth, smallWidth }
								, ImVec2{ (*m_spriteSheet)[tileNum].m_min.x, (*m_spriteSheet)[tileNum].m_max.y }
								, ImVec2{ (*m_spriteSheet)[tileNum].m_max.x, (*m_spriteSheet)[tileNum].m_min.y }))
							{
								m_currentKeyFrame.m_frame = tileNum;
								m_currentState.AddFrame(m_currentKeyFrame);
							}
							ImGui::PopID();
						}
					}
					ImGui::EndTable();
				}
			}

			//----------------------------------------------ADD FRAME----------------------------------------------
			ImGui::Text("Frame");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::InputInt("##FrameIndex", &m_currentKeyFrame.m_frame))
			{
				if (m_currentKeyFrame.m_frame < 0) m_currentKeyFrame.m_frame = 0;
			}
			ImGui::SameLine();
			ImGui::Text("Duration");
			ImGui::SameLine();
			
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::DragFloat("##FrameTime", &m_currentKeyFrame.m_time))
			{
				if (m_currentKeyFrame.m_time < 0.0f) m_currentKeyFrame.m_time = 0.0f;
			}

			if (ImGui::Button("Add KeyFrame"))
			{
				m_currentState.AddFrame(m_currentKeyFrame);
			}

			ImGui::Dummy(ImVec2(0.0f, 35.0f));

			//----------------------------------------------ANIM PREVIEW----------------------------------------------
			if (ImGui::Button("Preview"))
			{
				m_previewPlaying = true;
			}
			ImGui::Text("Preview Time");
			ImGui::SameLine();
			ImGui::Text("%.4f", m_previewTimeElapsed);
			if (m_previewPlaying)
			{
				m_previewTimeElapsed += static_cast<float>(TIME->GetUnscaledDeltaTime());
			}
			if (m_currentState.GetFrameCount())
			{
				ImGui::Image((ImTextureID)(uint64_t)ASSETMANAGER->GetTextureIndex(m_spriteSheet->GetPNGRef()), ImVec2{ normalWidth, normalWidth }
					, ImVec2{ (*m_spriteSheet)[m_currentState[m_previewIndex].m_frame].m_min.x, (*m_spriteSheet)[m_currentState[m_previewIndex].m_frame].m_max.y }
					, ImVec2{ (*m_spriteSheet)[m_currentState[m_previewIndex].m_frame].m_max.x, (*m_spriteSheet)[m_currentState[m_previewIndex].m_frame].m_min.y });

				if (m_previewTimeElapsed > m_currentState[m_previewIndex].m_time)
				{
					++m_previewIndex;
					m_previewTimeElapsed = 0.0f;
				}
				if (m_previewIndex >= m_currentState.GetFrameCount()) {
					m_previewPlaying = false;
					m_previewIndex = 0;
				}
			}

			//----------------------------------------------KEYFRAME LIST----------------------------------------------
			ImGui::Text("Frames");

			ImGui::Text("Set All Duration");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(normalWidth);
			if (ImGui::DragFloat("##SetAllFrameTime", &m_allDuration))
			{

			}
			ImGui::SameLine();
			if (ImGui::Button("Set"))
			{
				for (int index{ 0 }; index < m_currentState.GetFrameCount(); ++index)
				{
					m_currentState[index].m_time = m_allDuration;
				}
			}

			for (int index{ 0 }; index < m_currentState.GetFrameCount(); ++index)
			{
				ImGui::Text("Frame");
				ImGui::SameLine();

				ImGui::PushID(index);
				m_tempKeyFrame.m_frame = m_currentState[index].m_frame;
				
				ImGui::SetNextItemWidth(normalWidth);
				if (ImGui::InputInt("##FrameIndex", &m_tempKeyFrame.m_frame))
				{
					if (m_tempKeyFrame.m_frame < 0) m_tempKeyFrame.m_frame = 0;
					m_currentState[index].m_frame = m_tempKeyFrame.m_frame;
				}
				ImGui::SameLine();

				ImGui::Text("Duration");
				ImGui::SameLine();
				m_tempKeyFrame.m_time = m_currentState[index].m_time;
				
				ImGui::SetNextItemWidth(normalWidth);
				if (ImGui::DragFloat("##FrameTime", &m_tempKeyFrame.m_time))
				{
					if (m_tempKeyFrame.m_time < 0.0f) m_tempKeyFrame.m_time = 0.0f;
					m_currentState[index].m_time = m_tempKeyFrame.m_time;
				}
				ImGui::SameLine();

				if (ImGui::Button("X"))
				{
					m_currentState.GetFrames().erase(m_currentState.GetFrames().begin() + index);
				}
				ImGui::SameLine();
				ImGui::Text("Delete");

				ImGui::PopID();
			}
		}
		//----------------------------------------------ANIMATION CONTROLLER EDITING----------------------------------------------
		else if (m_controllerLoaded)
		{
			//----------------------------------------------CONTROLLER NAME----------------------------------------------
			ImGui::Text("%-17s", "Name");
			ImGui::SameLine();
			ImGui::Text(m_currentController.GetName().c_str());

			//----------------------------------------------ADD STATE----------------------------------------------
			ImGui::Text("%-17s", "Add State");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(columnWidth);
			if (ImGui::BeginCombo("##State", "Select State"))
			{
				for (auto& [str, state] : ASSETMANAGER->AnimStates)
				{
					if (ImGui::Selectable(state.GetName().c_str()))
					{
						m_currentController.AddState(state.GetName());
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Dummy(ImVec2(0.0f, 35.0f));

			//----------------------------------------------CONTROLLER STATES----------------------------------------------
			ImGui::Text("States");
			for (int index{ 0 }; index < m_currentController.GetStateCount(); ++index)
			{
				ImGui::PushID(index);

				ImGui::Text(m_currentController[index].c_str());
				ImGui::SameLine();

				if (ImGui::Button("X"))
				{
					m_currentController.RemoveState(index);
				}
				ImGui::SameLine();
				ImGui::Text("Delete");

				ImGui::PopID();
			}
		}

		ImGui::End();
	}

	void EditorAnimationEditor::Save()
	{
		if (m_stateLoaded)
		{
			JSONSerializer::SerializeToFile(m_currentState.GetName().c_str(), m_currentState);

			// Update asset manager's anim state if it exists
			if (ASSETMANAGER->AnimStates.find(m_currentState.GetName()) != ASSETMANAGER->AnimStates.end())
			{
				ASSETMANAGER->AnimStates[m_currentState.GetName()] = m_currentState;
			}
		}
		else if (m_controllerLoaded)
		{
			JSONSerializer::SerializeToFile(m_currentController.GetName().c_str(), m_currentController);

			// Update asset manager's anim controller if it exists
			if (ASSETMANAGER->AnimControllers.find(m_currentController.GetName()) != ASSETMANAGER->AnimControllers.end())
			{
				ASSETMANAGER->AnimControllers[m_currentController.GetName()] = m_currentController;
			}
		}
	}

	void EditorAnimationEditor::LoadState(std::string const& name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_currentState);
		m_currentState.SetName(name);

		if (m_currentState.GetSpriteSheetName() != "None")
		{
			m_spriteSheet = &ASSETMANAGER->SpriteSheets[m_currentState.GetSpriteSheetName()];
		}
		m_stateLoaded = true;
		m_controllerLoaded = false;
	}

	void EditorAnimationEditor::LoadController(std::string const& name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_currentController);
		m_currentController.SetName(name);

		m_controllerLoaded = true;
		m_stateLoaded = false;
	}
}
