/*!************************************************************************
 \file				EditorProfiler.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This source file contains functions defintions for the profiler layer of the
 Editor. This is to print out the usage of each system per frame.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"

#include "EditorProfiler.h"
#include "implot.h"			// For graphing
#include <algorithm>		// For sorting timings

#include "EditorGameView.h"

namespace LB
{
	EditorProfiler* EDITORPROFILER = nullptr;

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorProfiler class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorProfiler::EditorProfiler(std::string layerName) : Layer(layerName), m_currentFrameHistoryIndex(0)
	{
		if (!EDITORPROFILER)
			EDITORPROFILER = this;
		else
			DebuggerLogError("Editor Profiler already exist!");

		// Serialize this in the future
		SetFrameHistorySize(80);
	}

	/*!***********************************************************************
	  \brief
	  Compares the timing between two frames
	  \return
	  True if frame timing of the left parameter is lower than that of the right,
	  else false.
	*************************************************************************/
	bool CompareFrameMoments(SystemFrameMoment const& lhs, SystemFrameMoment const& rhs) {
		return lhs.timing < rhs.timing;
	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorProfiler layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorProfiler::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Buttons in the top bar
		ImGui::Checkbox("Profile", &m_shouldProfile);

		ImGui::Separator();

		// If profiler is running, fit graph to window!
		if (m_shouldProfile) 
		{
			ImPlot::SetNextAxesToFit();
		}
		// Draw the profile graph
		if (ImPlot::BeginPlot("Profile Graph", ImGui::GetContentRegionAvail(), ImPlotAxisFlags_RangeFit | ImPlotFlags_NoLegend))
		{
			// Graph out the time taken for each system in this frame
			for (auto& frame : m_systemFrames)
			{
				ImPlot::PlotLine(frame.first.c_str(), &frame.second[0].offset, frame.second.size(), 1.0, 0.0, 0, m_currentFrameHistoryIndex, sizeof(SystemFrame));
			}

			// If current frame is hovered over
			if (ImPlot::IsPlotHovered()) {
				ImDrawList* drawList = ImPlot::GetPlotDrawList();
				ImPlotPoint mouse = ImPlot::GetPlotMousePos();

				// Calculate which index is hovered by the mouse
				int hoveredIndex = m_currentFrameHistoryIndex - (m_framesHistorySize - (int)mouse.x);
				if (hoveredIndex < 0) hoveredIndex += m_framesHistorySize;

				// If a valid frame is hovered,draw a "selected" rectangle and display info
				if (hoveredIndex >= 0 && hoveredIndex < m_framesHistorySize)
				{
					float  tool_l = ImPlot::PlotToPixels(mouse.x - 0.5, mouse.y).x;
					float  tool_r = ImPlot::PlotToPixels(mouse.x + 0.5, mouse.y).x;
					float  tool_t = ImPlot::GetPlotPos().y;
					float  tool_b = tool_t + ImPlot::GetPlotSize().y;

					ImPlot::PushPlotClipRect();
					drawList->AddRectFilled(ImVec2(tool_l, tool_t), ImVec2(tool_r, tool_b), IM_COL32(128, 128, 128, 128));
					ImPlot::PopPlotClipRect();

					// Get the timings of all systems this frame and sort it
					float totalTime{0};
					std::vector<SystemFrameMoment> timings;
					for (auto& frame : m_systemFrames)
					{
						timings.emplace_back(frame.first.c_str(), frame.second[hoveredIndex].timing);
						totalTime += frame.second[hoveredIndex].timing;
					}
					std::sort(timings.begin(), timings.end(), CompareFrameMoments);

					ImGui::BeginTooltip();
					for (SystemFrameMoment timing : timings)
					{
						ImGui::Text("%30s: %.4f ms | %3.2f%%", timing.name.c_str(), timing.timing, (timing.timing / totalTime) * 100.0f);
					}
					ImGui::Text("%30s: %.4f ms        ", "============Total============", totalTime);
					ImGui::EndTooltip();
				}
			}
			ImPlot::EndPlot();
		}

		ImGui::End();
	}

	/*!***********************************************************************
	  \brief
	  Update the system frames with timing data.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorProfiler::UpdateSystemFrames(std::map<std::string, double> const& timings)
	{
		// TO DO: Pause profiler on key press pause as well
		if (!m_shouldProfile) return;

		// Ring buffer, move the index by 1 to store current frame information
		m_currentFrameHistoryIndex = (m_currentFrameHistoryIndex + 1) % m_framesHistorySize;

		for (auto const& timing : timings)
		{
			// If the timing doesn't exist in the current map, add it first
			if (m_systemFrames.find(timing.first) == m_systemFrames.end())
			{
				std::vector<SystemFrame> systemFrameTimings(m_framesHistorySize);
				m_systemFrames[timing.first] = systemFrameTimings;
			}

			m_systemFrames[timing.first][m_currentFrameHistoryIndex].timing = (float)timing.second;
		}

		// In order to "stack" the lines onto of each other, there needs to be an offset
		float offset{ 0 };
		for (auto& frame : m_systemFrames)
		{
			frame.second[m_currentFrameHistoryIndex].offset = offset + frame.second[m_currentFrameHistoryIndex].timing;
			offset += frame.second[m_currentFrameHistoryIndex].timing;
		}
	}

	/*!***********************************************************************
	  \brief
	  Set the size of the frame history.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorProfiler::SetFrameHistorySize(int newSize)
	{
		m_framesHistorySize = newSize;
	}
}
