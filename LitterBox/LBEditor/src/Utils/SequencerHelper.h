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

namespace ImGui
{
	float& NeoGetZoom();

	ImVec2 NeoGetViewRange();
}
