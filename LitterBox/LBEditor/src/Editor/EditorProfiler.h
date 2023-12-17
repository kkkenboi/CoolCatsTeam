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

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB
{
	/*!***********************************************************************
	  \brief
	  Used for setting up the graph for system timings
	*************************************************************************/
	struct SystemFrame
	{
		float timing; // Actual timing
		float offset; // Timing + y-offset
	};

	/*!***********************************************************************
	  \brief
	  For this frame, holds the system's name and time spent.
	*************************************************************************/
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
		*************************************************************************/
		EditorProfiler(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorProfiler layer.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorProfiler layer.
		*************************************************************************/
		void Destroy() {}

		/*!***********************************************************************
		  \brief
		  Update the system frames with timing data.
		*************************************************************************/
		void UpdateSystemFrames();

		/*!***********************************************************************
		  \brief
		  Set the size of the frame history.
		*************************************************************************/
		void SetFrameHistorySize(int newSize);

	private:
		bool m_shouldProfile {false};									// Should the profiler graph the timings?

		std::map<std::string, std::vector<SystemFrame>> m_systemFrames;	// Timings for each system
		int m_framesHistorySize, m_currentFrameHistoryIndex;			// How many frames data to hold
	};

	/*!***********************************************************************
	 \brief
	 For event subscription, update the system frames with timing data.
	*************************************************************************/
	void UpdateSystemFrames();

	extern EditorProfiler* EDITORPROFILER;
}