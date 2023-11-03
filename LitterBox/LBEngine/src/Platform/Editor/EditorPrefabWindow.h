/*!************************************************************************
 \file				EditorPrefabWindow.h
 \author(s)			Amadeus Chia 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
 \brief

 This file contains functions declarations for the Editor prefab window
 TODO :: Implement the prefab preview here!! 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "Platform/Editor/Editor.h"
#include "LitterBox/Engine/Layer.h"

namespace LB
{
	class EditorPrefabWindow : public Layer
	{
		public:
		/*!***********************************************************************
		\brief
		Constructor for the EditorPrefabWindow class.
		(TODO:: Implement the prefab window preview here in the future!)
		\return
		Nothing.
		*************************************************************************/
		EditorPrefabWindow(std::string layerName);

		/*!***********************************************************************
		\brief
		Initializes the EditorPrefabWindow layer.
		\return
		Nothing.
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		\brief
		Initializes the EditorPrefabWindow layer. (Subscribes the callback function)
		\return
		Nothing.
		*************************************************************************/
		void UpdateLayer() override;

	private:

	};
}