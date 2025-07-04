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
		 Start function gets the collider component of the game object

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
		GameObject* line{ nullptr };
		GameObject* curtain{ nullptr };
		float time{};
		bool animFlag{ false };
		//-------------Rotation matrix values to get the left of the button---------------

		//------------------------For Quit Confirmation---------------------------
		CPCollider* mCollider{ nullptr };

		GameObject* ConfirmMenuTexture{nullptr};
		GameObject* ConfirmMenuYesButton{ nullptr };
		GameObject* ConfirmMenuNoButton{ nullptr };

		GameObject* SettingsMenuTexture{ nullptr };
		GameObject* SettingsMenuBack{ nullptr };
		GameObject* SettingsMenuFullscreen{ nullptr };

		GameObject* SettingsMenuMV{ nullptr };
		GameObject* SettingsMenuMVCollider{ nullptr };

		GameObject* SettingsMenuSFX{ nullptr };
		GameObject* SettingsMenuSFXCollider{ nullptr };

		GameObject* SettingsMenuMusic{ nullptr };
		GameObject* SettingsMenuMusicCollider{ nullptr };
		//------------------------For Quit Confirmation---------------------------

		//----------Flag to turn off menu-----------
		static bool menuFlag; //if true then we can interact with main menu
								//if false that means we have quit or settings open

		bool tutorial{ false }; //this flag is to indicate if the object, that the script is attached to, is tutorial collider then we turn it on to go to the tutorial page
	};
	REGISTER_SCRIPT(QuitScript)
}