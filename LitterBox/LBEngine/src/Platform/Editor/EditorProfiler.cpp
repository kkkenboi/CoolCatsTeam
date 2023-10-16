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
	EditorProfiler::EditorProfiler(std::string layerName) : Layer(layerName) 
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
		systemFrames.clear();

		// Add each system into the frame
		std::vector systemNames = CORE->GetAllSystemNames();
	}

	void EditorProfiler::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::ShowDemoWindow();

		ImGui::End();
	}

	void EditorProfiler::SetFrameHistorySize(int newSize)
	{
		framesHistorySize = newSize;
	}
}
