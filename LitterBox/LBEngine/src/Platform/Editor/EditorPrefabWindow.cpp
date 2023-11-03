/*!************************************************************************
 \file				EditorHierarchy.cpp
 \author(s)			Amadeus Chia 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This file contains functions defintions for the Editor prefab window
 TODO :: Implement the prefab preview here!! 

 
 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "pch.h"
#include "EditorPrefabWindow.h"

namespace LB
{
	/*!***********************************************************************
	  \brief
	  Constructor for the EditorPrefabWindow class.
	  (TODO:: Implement the prefab window preview here in the future!)
	  \return
	  Nothing.
	*************************************************************************/
	EditorPrefabWindow::EditorPrefabWindow(std::string layerName) : Layer(layerName)
	{
	}

	/*!***********************************************************************
	\brief
	Initializes the EditorPrefabWindow layer.
	\return
	Nothing.
	*************************************************************************/
	void EditorPrefabWindow::Initialize()
	{
	}

	/*!***********************************************************************
	\brief
	Initializes the EditorPrefabWindow layer. (Subscribes the callback function)
	\return
	Nothing.
	*************************************************************************/
	void EditorPrefabWindow::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::End();
	}
}