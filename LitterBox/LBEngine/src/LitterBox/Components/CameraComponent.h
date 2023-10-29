/*!************************************************************************
 \file				CameraComponent.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan\@digipen.edu
 \par Course:       CSD2401A
 \date				17/10/2023
 \brief
 This file id the header file containing the declerations for the in-game
 camera component.

 NOTE: It is important to have a camera component in the scene. If there is
 none, then the RenderSystem will not run.
**************************************************************************/

#pragma once
#include "TransformComponent.h"
#include "LitterBox/Utils/Math.h"

namespace LB {
	/*!***********************************************************************
	\brief
	 CPCamera is the scene camera for the game.
	*************************************************************************/
	class CPCamera : public IComponent 
	{
	public:
		void Initialise() override;

		Vec2<float> getCam();

		static ComponentTypeID GetType()
		{
			return C_CPCamera;
		}
	};
}