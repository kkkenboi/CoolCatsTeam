/*!************************************************************************
 \file                QuitScript.h
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                20-11-2023
 \brief

 Header file that contains the definition for the Quit Menu script to be used
 in game for the GUI

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class QuitScript : public CPPBehaviour
	{
		CPCollider* coll{ nullptr };
		/*!***********************************************************************
		\brief
		 State function gets the collider component of the game object

		 NOTE: The game object MUST HAVE the collider component, behaviour if not
		 is undefined
		*************************************************************************/
		void Start() override;

		/*!***********************************************************************
		\brief
		 Update function will perform the mouse click check on the UI button and
		 perform the specified instructions.
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		 Empty Destroy function. No purpose
		*************************************************************************/
		void Destroy() override;

		//-------------Rotation matrix values to get the left of the button---------------
		Vec2<float> rot_row1{};
		Vec2<float> rot_row2{};
		Vec2<float> right_side{};

		GameObject* hand{ nullptr };
		//-------------Rotation matrix values to get the left of the button---------------
	};
}