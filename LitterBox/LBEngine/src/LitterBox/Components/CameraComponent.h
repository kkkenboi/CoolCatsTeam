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

	//CPCamera* game_cam{ nullptr };

	/*!***********************************************************************
	\brief
	 CPCamera is the scene camera for the game.
	*************************************************************************/
	class CPCamera : public IComponent 
	{
		Vec3<float> position;

		CPCamera() : position{0.f,0.f,5.f} {
			/*if (!game_cam)
				game_cam = this;
			else
				DebuggerLogError("Camera component already exists. Ignoring newly made camera");*/
		}

		void Initialise() override
		{
			
		}
	};
}