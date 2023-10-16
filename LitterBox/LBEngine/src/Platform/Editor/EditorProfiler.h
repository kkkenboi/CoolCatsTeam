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
	struct EditorProfilerSystem
	{
		std::string name;
		std::vector<float> frameTimes;
	};

	class EditorProfiler : public Layer
	{
	public:
		EditorProfiler(std::string layerName);

		void Initialize() override;

		void InitializeSystemFrames();

		void UpdateLayer() override;

		void SetFrameHistorySize(int newSize);

	private:
		std::vector<EditorProfilerSystem> systemFrames;
		int framesHistorySize, currentFrameHistoryIndex;
	};
}