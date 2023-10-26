/*!************************************************************************
 \file                Scene.h
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
  This file contains the functions for the Scene. A scene can be thought of
  as the root game object for that holds every other game object. It is 
  then managed by the Scene Manager

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "LitterBox/Components/TransformComponent.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Scene class that holds data needed for a scene
	*************************************************************************/
	class Scene
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initialises the Scene
		*************************************************************************/
		virtual void Init() {}

		/*!***********************************************************************
		 \brief
		 Update function for the scene
		*************************************************************************/
		virtual void Update() {}

		/*!***********************************************************************
		 \brief
		 Destroy function that handles the destruction of the scene
		*************************************************************************/
		virtual void Destroy() {}

		void Serialize()
		{

		}

		void Deserialize()
		{

		}

	protected:
		std::string name;
		CPTransform* root;	// All game objects are children of this root game object
	};
}
