/*!************************************************************************
 \file				EditorAnimationEditor.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the EditorAnimationEditor class and all its functionalities 
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

#include "Utils/SequencerHelper.h" // Helper functions for neo sequencer

/*!***********************************************************************
\brief
Constructor of the Animation Editor
*************************************************************************/

namespace LB
{
	static float columnWidth = 60.0f;
	static float normalWidth = 150.f;
	static float smallWidth = 60.f;

	/*!***********************************************************************
	  \brief
	  Sets up the default layer name.
	*************************************************************************/
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
		if (m_stateLoaded)
		{
			UpdateLoadedState();
		}
		else if (m_controllerLoaded)
		{
			UpdateLoadedController();
		}
		ImGui::End();
	}

	void EditorAnimationEditor::UpdateLoadedState()
	{
		if (ImGui::Button("Save"))
		{
			Save();
		}

		// For playing/pausing the timeline 
		if (INPUT->IsKeyTriggered(KeyCode::KEY_SPACE))
		{
			m_previewPlaying = !m_previewPlaying;
		}

		// If previewing, update the current frame
		if (m_previewPlaying)
		{
			m_elapsedTime += static_cast<float>(TIME->GetUnscaledDeltaTime());
			if (m_elapsedTime >= m_frameDuration)
			{
				m_elapsedTime -= m_frameDuration;

				++m_currentFrame;
				if (m_currentFrame > m_loadedState.m_endFrame)
				{
					m_currentFrame = m_loadedState.m_startFrame;
				}

				m_loadedState.Update();
			}
		}

		// State frame control
		ImGui::Separator();
		ImGui::SetNextItemWidth(smallWidth);
		ImGui::DragInt("Start Frame", &m_loadedState.m_startFrame);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(smallWidth);
		ImGui::DragInt("End Frame", &m_loadedState.m_endFrame);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		//----------------------------------------------ANIM VALUE INPUTS----------------------------------------------
		ImGui::Dummy({ 0.0f, 35.0f });

		m_editActive = m_loadedState.m_active.GetCurrentExact(m_currentFrame);
		if (ImGui::Checkbox("##Active", &m_editActive))
		{
			m_loadedState.m_active.Insert(LBKeyFrame<bool>{m_currentFrame, m_editActive});
			m_loadedState.UpdateLastFrame(m_currentFrame);
			m_loadedState.Start(m_currentFrame);
		}

		ImGui::Dummy({ 0.0f, 32.5f });

		m_editPosX = m_loadedState.m_pos.GetCurrentExact(m_currentFrame).x;
		if (ImGui::DragFloat("##PosX", &m_editPosX))
		{
			m_loadedState.m_pos.Insert(LBKeyFrame<Vec2<float>>{m_currentFrame, { m_editPosX, 0.0f }});
			m_loadedState.UpdateLastFrame(m_currentFrame);
			m_loadedState.Start(m_currentFrame);
		}

		m_editPosY = m_loadedState.m_pos.GetCurrentExact(m_currentFrame).y;
		if (ImGui::DragFloat("##PosY", &m_editPosY))
		{
			m_loadedState.m_pos.Insert(LBKeyFrame<Vec2<float>>{m_currentFrame, { 0.0f, m_editPosY }});
			m_loadedState.UpdateLastFrame();
			m_loadedState.Start(m_currentFrame);
		}

		ImGui::Dummy({ 0.0f, 10.0f });

		m_editScaleX = m_loadedState.m_scale.GetCurrentExact(m_currentFrame).x;
		if (ImGui::DragFloat("##ScaleX", &m_editScaleX))
		{
			m_loadedState.m_scale.Insert(LBKeyFrame<Vec2<float>>{m_currentFrame, { m_editScaleX, 0.0f }});
			m_loadedState.UpdateLastFrame();
			m_loadedState.Start(m_currentFrame);
		}

		m_editScaleY = m_loadedState.m_scale.GetCurrentExact(m_currentFrame).y;
		if (ImGui::DragFloat("##ScaleY", &m_editScaleY))
		{
			m_loadedState.m_scale.Insert(LBKeyFrame<Vec2<float>>{m_currentFrame, { 0.0f, m_editScaleY }});
			m_loadedState.UpdateLastFrame();
			m_loadedState.Start(m_currentFrame);
		}

		m_editRot = m_loadedState.m_rot.GetCurrentExact(m_currentFrame);
		if (ImGui::DragFloat("##Rotation", &m_editRot))
		{
			m_loadedState.m_rot.Insert(LBKeyFrame<float>{m_currentFrame, m_editRot});
			m_loadedState.UpdateLastFrame();
			m_loadedState.Start(m_currentFrame);
		}

		m_editSprite = m_loadedState.m_sprite.GetCurrentExact(m_currentFrame);
		if (ImGui::DragInt("##Sprite", &m_editSprite))
		{
			m_loadedState.m_sprite.Insert(LBKeyFrame<int>{m_currentFrame, m_editSprite});
			m_loadedState.UpdateLastFrame();
			m_loadedState.Start(m_currentFrame);
		}

		ImGui::NextColumn();

		//----------------------------------------------ANIM TIMELINE----------------------------------------------
		ImGui::BeginChild("SequencerTable", ImVec2(0, 250), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		ImGui::FrameIndexType currentFrame = m_currentFrame;
		if (ImGui::BeginNeoSequencer("Sequencer", &m_currentFrame, &m_startFrame, &m_endFrame, { 0, 0 },
			ImGuiNeoSequencerFlags_AllowLengthChanging | ImGuiNeoSequencerFlags_EnableSelection | ImGuiNeoSequencerFlags_Selection_EnableDragging | ImGuiNeoSequencerFlags_Selection_EnableDeletion)) {
			if (ImGui::BeginTimeline("Set Active", m_loadedState.m_active))
			{
				CheckDeleteKeyFrame(m_loadedState.m_active);
				ImGui::EndNeoTimeLine();
			}
			if (ImGui::BeginNeoGroup("Transform", &m_transformOpen))
			{
				if (ImGui::BeginNeoGroup("Position", &m_posOpen))
				{
					if (ImGui::BeginTimeline("X", m_loadedState.m_pos))
					{
						CheckDeleteKeyFrame(m_loadedState.m_pos);
						ImGui::EndNeoTimeLine();
					}
					if (ImGui::BeginTimeline("Y", m_loadedState.m_pos))
					{
						CheckDeleteKeyFrame(m_loadedState.m_pos);
						ImGui::EndNeoTimeLine();
					}
					ImGui::EndNeoGroup();
				}
				if (ImGui::BeginNeoGroup("Scale", &m_scaleOpen))
				{
					if (ImGui::BeginTimeline("X", m_loadedState.m_scale))
					{
						CheckDeleteKeyFrame(m_loadedState.m_scale);
						ImGui::EndNeoTimeLine();
					}
					if (ImGui::BeginTimeline("Y", m_loadedState.m_scale))
					{
						CheckDeleteKeyFrame(m_loadedState.m_scale);
						ImGui::EndNeoTimeLine();
					}
					ImGui::EndNeoGroup();
				}
				if (ImGui::BeginTimeline("Rotation", m_loadedState.m_rot))
				{
					CheckDeleteKeyFrame(m_loadedState.m_rot);
					ImGui::EndNeoTimeLine();
				}
				ImGui::EndNeoGroup();
			}
			if (ImGui::BeginTimeline("Sprite", m_loadedState.m_sprite))
			{
				CheckDeleteKeyFrame(m_loadedState.m_sprite);
				ImGui::EndNeoTimeLine();
			}
			ImGui::EndNeoSequencer();
		}
		ImGui::EndChild();

		// If the player dragged the sequencer, update the current frame
		if (currentFrame != m_currentFrame)
		{
			m_loadedState.Start(m_currentFrame);
		}

		ImGui::Columns(1);
		ImGui::Separator();

		//----------------------------------------------SPRITESHEET SELECTION----------------------------------------------
		ImGui::Text("%-17s", "Spritesheet");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(normalWidth);
		if (ImGui::BeginCombo("##Spritesheet", (m_spriteSheet ? m_spriteSheet->GetName().c_str() : "None") ))
		{
			for (auto& [str, sSheet] : ASSETMANAGER->SpriteSheets)
			{
				std::filesystem::path tempPath{ str };
				if (ImGui::Selectable(tempPath.filename().stem().string().c_str()))
				{
					m_spriteSheet = &sSheet;
					m_loadedState.m_spriteSheetName = sSheet.GetName();
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
							m_loadedState.m_sprite.Insert(LBKeyFrame<int>{m_currentFrame, tileNum});
							m_loadedState.UpdateLastFrame();
							m_loadedState.Start(m_currentFrame);
						}
						ImGui::PopID();
					}
				}
				ImGui::EndTable();
			}
		}

		if (ImGui::CollapsingHeader("Debugging Info"))
		{
			ImGui::Text("Preview Curr %d", m_currentFrame);
			ImGui::SameLine();
			ImGui::Text("State Curr %d", m_loadedState.m_currentFrame);

			ImGui::Text("Pos list");
			ImGui::Text("Pos current %d, next %d", m_loadedState.m_pos.m_currentIndex, m_loadedState.m_pos.m_nextIndex);
			for (int index{ 0 }; index < m_loadedState.m_pos.GetData().Size(); ++index)
			{
				ImGui::Text("Frame %d, State %.2f %.2f", m_loadedState.m_pos.GetData()[index].m_frame, m_loadedState.m_pos.GetData()[index].m_data.x, m_loadedState.m_pos.GetData()[index].m_data.y);
			}
		}
	}

	void EditorAnimationEditor::UpdateLoadedController()
	{
		if (ImGui::Button("Save"))
		{
			Save();
		}
		ImGui::Separator();

		//----------------------------------------------CONTROLLER NAME----------------------------------------------
		ImGui::Text("%-17s", "Name");
		ImGui::SameLine();
		ImGui::Text(m_currentController.m_name.c_str());

		//----------------------------------------------ADD STATE----------------------------------------------
		ImGui::Text("%-17s", "Add State");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(columnWidth);
		if (ImGui::BeginCombo("##State", "Select State"))
		{
			for (auto& [str, state] : ASSETMANAGER->AnimStates)
			{
				if (ImGui::Selectable(state.m_name.c_str()))
				{
					m_currentController.AddState(state.m_name);
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Dummy(ImVec2(0.0f, 35.0f));

		//----------------------------------------------CONTROLLER STATES----------------------------------------------
		ImGui::Text("States");
		for (int index{ 0 }; index < m_currentController.Count(); ++index)
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

	/*!***********************************************************************
	  \brief
	  Saves the currently loaded animation state/controller.
	*************************************************************************/
	void EditorAnimationEditor::Save()
	{
		if (m_stateLoaded)
		{
			JSONSerializer::SerializeToFile(m_loadedState.m_name.c_str(), m_loadedState);

			// Update asset manager's anim state if it exists
			if (ASSETMANAGER->AnimStates.find(m_loadedState.m_name) != ASSETMANAGER->AnimStates.end())
			{
				ASSETMANAGER->AnimStates[m_loadedState.m_name] = m_loadedState;
			}
		}
		else if (m_controllerLoaded)
		{
			JSONSerializer::SerializeToFile(m_currentController.m_name.c_str(), m_currentController);

			// Update asset manager's anim controller if it exists
			if (ASSETMANAGER->AnimControllers.find(m_currentController.m_name) != ASSETMANAGER->AnimControllers.end())
			{
				ASSETMANAGER->AnimControllers[m_currentController.m_name] = m_currentController;
			}
		}
	}


	/*!***********************************************************************
	  \brief
	  Loads the animation state for editing.
	*************************************************************************/
	void EditorAnimationEditor::LoadState(std::string const& name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_loadedState);

		if (m_loadedState.m_spriteSheetName != "None")
		{
			m_spriteSheet = &ASSETMANAGER->SpriteSheets[m_loadedState.m_spriteSheetName];
		}
		m_stateLoaded = true;
		m_controllerLoaded = false;
	}

	/*!***********************************************************************
	  \brief
	  Loads the animation controller for editing.
	*************************************************************************/
	void EditorAnimationEditor::LoadController(std::string const& name)
	{
		JSONSerializer::DeserializeFromFile(name.c_str(), m_currentController);
		m_currentController.m_name = name;

		m_controllerLoaded = true;
		m_stateLoaded = false;
	}
}
