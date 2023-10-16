/*!************************************************************************
 \file				EditorConsole.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorConsole.h"

namespace LB
{
	EditorConsole* EDITORCONSOLE = nullptr;

	EditorConsole::EditorConsole(std::string layerName) : Layer(layerName) 
	{
		if (!EDITORCONSOLE)
			EDITORCONSOLE = this;
		else
			DebuggerLogError("Editor Console already exist!");

		// Add the colors into the lookup table (Serialize this in the future)
		messageColors[EditorConsoleMsgType::DEBUG]		= ImVec4(0.25f, 0.56f, 0.4f, 1.0f);
		messageColors[EditorConsoleMsgType::WARNING]	= ImVec4(0.8f, 0.8f, 0.28f, 1.0f);
		messageColors[EditorConsoleMsgType::ERROR]		= ImVec4(0.8f, 0.27f, 0.25f, 1.0f);
	}

	void EditorConsole::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Buttons and filter box at the top of the console
		if (ImGui::Button("Clear")) 
		{
			messages.clear();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Error Pause", &pauseOnError);
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 240.0f);
		messageFilter.Draw("Filter", 180.0f);

		ImGui::Separator();

		// Message logs
		const float consoleLogHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -consoleLogHeight), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            for (ConsoleMessage const& message : messages)
            {
                if (!messageFilter.PassFilter(message.msg.c_str()))
                    continue;

				ImGui::PushStyleColor(ImGuiCol_Text, messageColors[message.type]);
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

	void EditorConsole::AddLogMessage(std::string const& log)
	{
		messages.emplace_back(log, EditorConsoleMsgType::DEBUG);
	}

	void EditorConsole::AddWarningMessage(std::string const& warning)
	{
		messages.emplace_back(warning, EditorConsoleMsgType::WARNING);
	}

	void EditorConsole::AddErrorMessage(std::string const& error)
	{
		messages.emplace_back(error, EditorConsoleMsgType::ERROR);
	}
}
