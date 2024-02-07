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
			ImGui::Text("Frame");
			ImGui::SameLine();
			if (ImGui::InputInt("##FrameIndex", &m_currentKeyFrame.m_frame))
			{

			}
			ImGui::SameLine();
			ImGui::Text("Time");
			ImGui::SameLine();
			if (ImGui::InputFloat("##FrameTime", &m_currentKeyFrame.m_time))
			{

			}

			if (ImGui::Button("Add KeyFrame"))
			{
				m_currentState.AddFrame(m_currentKeyFrame);
			}

			ImGui::SeparatorEx(ImGuiSeparatorFlags_None);
			ImGui::Text("Frames");
			for (int index{ 0 }; index < m_currentState.GetFrameCount(); ++index)
			{
				ImGui::Text("Frame");
				ImGui::SameLine();
				if (ImGui::InputInt("##FrameIndex", &m_currentKeyFrame.m_frame))
				{

				}
				ImGui::SameLine();
				ImGui::Text("Time");
				ImGui::SameLine();
				if (ImGui::InputFloat("##FrameTime", &m_currentKeyFrame.m_time))
				{

				}
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
