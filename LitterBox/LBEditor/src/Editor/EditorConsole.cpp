/*!************************************************************************
 \file				EditorConsole.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file contains functions definitions for the console layer of the
 Editor. This is to show the logs and any errors that may arise during the runtime
 of the Editor.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorConsole.h"

namespace LB
{
	EditorConsole* EDITORCONSOLE = nullptr;

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorConsole class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorConsole::EditorConsole(std::string layerName) : Layer(layerName)
	{
		if (!EDITORCONSOLE)
			EDITORCONSOLE = this;
		else
			DebuggerLogError("Editor Console already exist!");

		m_pauseOnError = false;

		// Add the colors into the lookup table (Serialize this in the future)
		m_messageColors[EditorConsoleMsgType::DEBUG]		= ImVec4(0.25f, 0.56f, 0.4f, 1.0f);
		m_messageColors[EditorConsoleMsgType::WARNING]		= ImVec4(0.8f, 0.8f, 0.28f, 1.0f);
		m_messageColors[EditorConsoleMsgType::ERROR]		= ImVec4(0.8f, 0.27f, 0.25f, 1.0f);

		// Add debug log message events
		DEBUG->onDebugLog.Subscribe(LB::AddLogMessage);
		DEBUG->onDebugWarning.Subscribe(LB::AddWarningMessage);
		DEBUG->onDebugError.Subscribe(LB::AddErrorMessage);
	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorConsole layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorConsole::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Buttons and filter box at the top of the console
		if (ImGui::Button("Clear")) 
		{
			m_messages.clear();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Error Pause", &m_pauseOnError);
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 240.0f);
		m_messageFilter.Draw("Filter", 180.0f);

		ImGui::Separator();

		// Message logs
		const float consoleLogHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -consoleLogHeight), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            for (ConsoleMessage const& message : m_messages)
            {
                if (!m_messageFilter.PassFilter(message.msg.c_str()))
                    continue;

				ImGui::PushStyleColor(ImGuiCol_Text, m_messageColors[message.type]);
                ImGui::TextUnformatted(message.msg.c_str());
				ImGui::PopStyleColor();
			}

			// If the current scroll position is at the bottom, remain at the bottom
			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
        }
		ImGui::EndChild();

		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Destroys the EditorConsole layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorConsole::Destroy()
	{
		DEBUG->onDebugLog.Unsubscribe(LB::AddLogMessage);
		DEBUG->onDebugWarning.Unsubscribe(LB::AddWarningMessage);
		DEBUG->onDebugError.Unsubscribe(LB::AddErrorMessage);
		EDITORCONSOLE = nullptr;
	}

	/*!***********************************************************************
	  \brief
	  Add a log message to the console.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorConsole::AddLogMessage(std::string const& log)
	{
		m_messages.emplace_back(log, EditorConsoleMsgType::DEBUG);
	}

	/*!***********************************************************************
	  \brief
	  Add a warning message to the console.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorConsole::AddWarningMessage(std::string const& warning)
	{
		m_messages.emplace_back(warning, EditorConsoleMsgType::WARNING);
	}

	/*!***********************************************************************
	  \brief
	  Add an error message to the console.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorConsole::AddErrorMessage(std::string const& error)
	{
		m_messages.emplace_back(error, EditorConsoleMsgType::ERROR);
	}

	void AddLogMessage(std::string const& log)
	{
		EDITORCONSOLE->AddLogMessage(log);
	}

	void AddWarningMessage(std::string const& warning)
	{
		EDITORCONSOLE->AddWarningMessage(warning);
	}

	void AddErrorMessage(std::string const& error)
	{
		EDITORCONSOLE->AddErrorMessage(error);
	}
}
