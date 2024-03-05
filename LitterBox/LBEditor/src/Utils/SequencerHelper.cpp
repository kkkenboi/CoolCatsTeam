/*!************************************************************************
 \file				SequencerHelper.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				01-03-2023
 \brief

 This source file extends the functionality of the ImGui Neo Sequencer library.
 The library is good but pretty basic so this file is needed to do the features
 that we want.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "SequencerHelper.h"

#include "imgui_internal.h"
#include "imgui_neo_internal.h"
#include <unordered_map>

namespace ImGui
{
    float& NeoGetZoom()
    {
        IM_ASSERT(inSequencer && "Not in active sequencer!");
        auto& context = sequencerData[currentSequencer];

        return context.Zoom;
    }

    ImVec2 NeoGetViewRange()
    {
		IM_ASSERT(inSequencer && "Not in active sequencer!");
		auto& context = sequencerData[currentSequencer];

        const auto viewStart = context.StartFrame + context.OffsetFrame;
        const auto viewEnd = viewStart + context.EndFrame;

		return ImVec2(viewStart, viewStart + context.EndFrame);
	}

    /*template <>
    bool BeginTimeline(const char* label, LB::LBKeyFrameGroup<LB::Vec2<float>>& keyframes, bool* open, ImGuiNeoTimelineFlags flags)
    {
        std::vector<int32_t*> c_keyframes{keyframes.GetData().Size()};
        for (uint32_t i = 0; i < c_keyframes.size(); ++i)
            c_keyframes[i] = &keyframes.GetData()[i].m_frame;

        return BeginNeoTimeline(label, c_keyframes.data(), c_keyframes.size(), open, flags);
    }*/
}

