/*!************************************************************************
 \file				EditorSceneView.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorSceneView.h"

namespace LB
{
	EditorSceneView::EditorSceneView(std::string layerName) : Layer(layerName) {}

	void EditorSceneView::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		ImGui::End();
	}
}