/*!************************************************************************
 \file				EditorAssets.cpp
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
#include "EditorAssets.h"

namespace LB
{
	EditorAssets::EditorAssets(std::string layerName) : Layer(layerName) {}

	void EditorAssets::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::End();
	}
}
