/*!************************************************************************
 \file				CameraComponent.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan\@digipen.edu
 \par Course:       CSD2401A
 \date				17/10/2023
 \brief
 This file id the header file containing the declerations for the in-game
 camera component.

 NOTE: Currently not used as the game view is still only one map.
	   The implementation is defined in Renderer.cpp
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
		/*!***********************************************************************
		\brief
		 Function initialises the game camera object in the scene to a single
		 camera.

		 NOTE: you can have multiple camera components in multiple game objects
		 but only one will affect the actual game camera
		*************************************************************************/
		void Initialise() override;

		/*!***********************************************************************
		\brief
		 Function to get the position coordinates of the transform component of
		 the game object that the game camera is attached to.

		\return
		 The x and y coordinates of the game object
		*************************************************************************/
		Vec2<float> getCam();


		/*!***********************************************************************
		\brief
		 Allows one to get the type of the camera component

		\return
		 The Camera component
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPCamera;
		}
	};
}