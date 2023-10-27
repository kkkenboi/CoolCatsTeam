/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions defintions for the hierarchy layer of the
 Editor. This is to ensure that there is a overview of all the game objects
 in the game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "EditorHierarchy.h"

namespace LB
{
	EditorHierarchy::EditorHierarchy(std::string layerName) : Layer(layerName) {}

	void EditorHierarchy::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		if (ImGui::Button("Create Game Object"))
		{
			std::cout << "Hello world :D\n";
		}
		ImGui::End();
	}
}
