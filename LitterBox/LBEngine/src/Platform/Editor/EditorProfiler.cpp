/*!************************************************************************
 \file				EditorProfiler.cpp
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
#include "EditorProfiler.h"
#include "LitterBox/Core/Core.h" // For getting system names

namespace LB
{
	EditorProfiler* EDITORPROFILER = nullptr;

	EditorProfiler::EditorProfiler(std::string layerName) : Layer(layerName), m_currentFrameHistoryIndex(0)
	{
		if (!EDITORPROFILER)
			EDITORPROFILER = this;
		else
			DebuggerLogError("Editor Profiler already exist!");

		// Serialize this in the future
		SetFrameHistorySize(120);
	}

	void EditorProfiler::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Buttons in the top bar
		ImGui::Checkbox("Profile", &m_shouldProfile);

		ImGui::Separator();

		// Graph out the time taken for each system in this frame
		for (auto & frame : m_systemFrames)
		{

		}

		//ImGui::ShowDemoWindow();
		ImGui::End();
	}

	void EditorProfiler::UpdateSystemFrames(std::map<std::string, double> const& timings)
	{
		// Ring buffer, move the index by 1 to store current frame information
		m_currentFrameHistoryIndex = (m_currentFrameHistoryIndex + 1) % m_framesHistorySize;

		for (auto const& timing : timings)
		{
			// If the timing doesn't exist in the current map, add it first
			if (m_systemFrames.find(timing.first) == m_systemFrames.end())
			{
				std::vector<float> systemFrameTimings(120);
				m_systemFrames[timing.first] = systemFrameTimings;
			}

			m_systemFrames[timing.first][m_currentFrameHistoryIndex] = (float)timing.second;
		}
	}

	void EditorProfiler::SetFrameHistorySize(int newSize)
	{
		m_framesHistorySize = newSize;
	}
}
