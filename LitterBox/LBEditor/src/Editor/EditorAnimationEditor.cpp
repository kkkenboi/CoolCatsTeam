/*!************************************************************************
 \file				EditorAnimationEditor.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the EditorAniamtionEditor class and all its functionalities 
 for the aniamtion view layer of the Editor.

 Editor.
  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "EditorAnimationEditor.h"

/*!***********************************************************************
\brief
Constructor of the Animation Editor
*************************************************************************/
LB::EditorAnimationEditor::EditorAnimationEditor(std::string layerName) : Layer(layerName)
{
	
}

//test listing for list box
const char* list[] = { "First texture", "second texture" };
int currItem{ 0 };

/*!***********************************************************************
\brief
 Updating the layers of the animator editor
*************************************************************************/
void LB::EditorAnimationEditor::UpdateLayer()
{
	ImGui::Begin(GetName().c_str());

	ImGui::BeginChild("AnimationEditor");
	ImVec2 wsize = ImGui::GetWindowSize();

	ImGui::Combo("Texture", &currItem, list, IM_ARRAYSIZE(list));

	ImGui::EndChild();

	ImGui::End();
}


