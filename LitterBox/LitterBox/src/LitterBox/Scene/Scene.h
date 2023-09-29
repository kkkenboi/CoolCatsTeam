/*!************************************************************************
 \file                Scene.h
 \author(s)           Ang Jiawei Jarrett
 \par DP email(s):    a.jiaweijarrett@digipen.edu
 \par Course:         CSD2401A
 \date                27-09-2023
 \brief
 The functions in the Scene class include:

**************************************************************************/

#pragma once

#include <list>
#include "LitterBox/Factory/Components.h"

namespace LB
{
	/*!***********************************************************************
	 \brief

	*************************************************************************/
	class Scene
	{
	public:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		virtual void Init() {};

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		virtual void Update() {};

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		virtual void Destroy() {};

	private:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::string name;

		// Hierachy will be done in M2
		// GameObject* gameObjs; // All game objects are children of this root game object
	};

	//bool operator==(Scene const& lhs, Scene const& rhs)
	//{

	//}
}
