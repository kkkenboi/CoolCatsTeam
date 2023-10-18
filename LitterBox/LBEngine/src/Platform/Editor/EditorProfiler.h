/*!************************************************************************
 \file				EditorProfiler.h
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	struct SystemFrame
	{
		std::string name;
		double currentFrameTiming;
	};

	class EditorProfiler : public Layer
	{
	public:
		EditorProfiler(std::string layerName);

		void UpdateLayer() override;

		void UpdateSystemFrames(std::map<std::string, double> const& timings);

		void SetFrameHistorySize(int newSize);

	private:
		bool m_shouldProfile;

		std::map<std::string, std::vector<double>> m_systemFrames;
		int m_framesHistorySize, m_currentFrameHistoryIndex;
	};

	extern EditorProfiler* EDITORPROFILER;
}