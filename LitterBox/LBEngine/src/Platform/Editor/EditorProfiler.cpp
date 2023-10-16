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
	EditorProfiler::EditorProfiler(std::string layerName) : Layer(layerName), m_currentFrameHistoryIndex(0)
	{
		// Serialize this in the future
		SetFrameHistorySize(120);
	}

	void EditorProfiler::Initialize()
	{
		InitializeSystemFrames();
	}

	void EditorProfiler::InitializeSystemFrames()
	{
		// Clear the previous vector just in case
		m_systemFrames.clear();

		// Add each system into the frame
		std::vector<std::string> systemNames = CORE->GetAllSystemNames();
		for (std::string const& name : systemNames)
		{
			m_systemFrames[name].resize(m_framesHistorySize);
		}
	}

	void EditorProfiler::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		// Graph out the time taken for each system in this frame

		ImGui::ShowDemoWindow();

		ImGui::End();
	}

	void EditorProfiler::UpdateSystemFrames(std::vector<SystemFrame> const& timings)
	{
		// Ring buffer, move the index by 1 to store current frame information
		m_currentFrameHistoryIndex = (m_currentFrameHistoryIndex + 1) % m_framesHistorySize;

		for (SystemFrame const& timing : timings)
		{
			m_systemFrames[timing.name][m_currentFrameHistoryIndex] = timing.currentFrameTiming;
		}
	}

	void EditorProfiler::SetFrameHistorySize(int newSize)
	{
		m_framesHistorySize = newSize;
	}
}
