/*!************************************************************************
 \file				SequencerHelper.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				01-03-2023
 \brief
 
 This header file extends the functionality of the ImGui Neo Sequencer library.
 The library is good but pretty basic so this file is needed to do the features
 that we want.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "imgui_neo_sequencer.h"
#include <LitterBox/Animation/AnimationState.h>

namespace ImGui
{
	/*!***********************************************************************
	  \brief
	  Gets the current zoom level of the timeline editor.
	*************************************************************************/
	float& NeoGetZoom();

	/*!***********************************************************************
	  \brief
	  From which frame to which frame is currently viewed in the timeline editor?
	*************************************************************************/
	ImVec2 NeoGetViewRange();

	/*!***********************************************************************
	  \brief
	  Overloaded BeginTimeline that takes in LBKeyFrameGroup type instead of
	  a vector.
	*************************************************************************/
	template <typename T>
	bool BeginTimeline(const char* label, LB::LBKeyFrameGroup<T>& keyframes, bool* open = nullptr, ImGuiNeoTimelineFlags flags = ImGuiNeoTimelineFlags_None)
	{
		std::vector<int32_t*> c_keyframes{keyframes.GetData().Size(), nullptr};
		for (size_t i = 0; i < c_keyframes.size(); ++i)
		{
			c_keyframes[i] = &keyframes.GetData()[i].m_frame;
		}

		return BeginNeoTimeline(label, c_keyframes.data(), c_keyframes.size(), open, flags);
	}
	
	/*template <>
	bool BeginTimeline(const char* label, LB::LBKeyFrameGroup<LB::Vec2<float>>& keyframes);*/
}
