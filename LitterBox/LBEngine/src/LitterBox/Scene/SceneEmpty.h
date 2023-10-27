/*!************************************************************************
 \file				EmptyScene.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				10-10-2023
 \brief
 This scene acts as the default scene for the scene manager to load until 
 the user specifies which scene they want.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "Scene.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Empty scene, Scene Manager needs to load a scene first, so an empty
	 scene will be loaded until the user gives a scene they want to load
	*************************************************************************/
	class SceneEmpty : public Scene
	{
	public:
		/*!***********************************************************************
		 \brief
		 Updates the scene
		*************************************************************************/
		void Update() override {

		}
	};
}
