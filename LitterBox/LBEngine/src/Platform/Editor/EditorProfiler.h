/*!************************************************************************
 \file				EditorProfiler.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This header file contains functions declarations for the profiler layer of the
 Editor. This is to print out the usage of each system per frame.


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
		float timing; // Actual timing
		float offset; // Timing + y-offset
	};

	struct SystemFrameMoment
	{
		std::string name;
		float timing;
	};

	class EditorProfiler : public Layer
	{
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorProfiler class.
		  \return
		  Nothing.
		*************************************************************************/
		EditorProfiler(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorProfiler layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorProfiler layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy() {}

		/*!***********************************************************************
		  \brief
		  Update the system frames with timing data.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateSystemFrames(std::map<std::string, double> const& timings);

		/*!***********************************************************************
		  \brief
		  Set the size of the frame history.
		  \return
		  Nothing.
		*************************************************************************/
		void SetFrameHistorySize(int newSize);

	private:
		bool m_shouldProfile {false};

		std::map<std::string, std::vector<SystemFrame>> m_systemFrames;
		int m_framesHistorySize, m_currentFrameHistoryIndex;
	};

	extern EditorProfiler* EDITORPROFILER;
}