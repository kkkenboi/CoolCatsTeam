/*!************************************************************************
 \file                CPPSSettings.h
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                2-4-2024
 \brief

 Header file that contains the definition for the Settings menu to be
 used for the game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Audio/AudioManager.h"

namespace LB
{
	class CPPSSettings : public CPPBehaviour
	{
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
		void Destroy() override {};

		//pointer to the game objects in the scene
		GameObject* SettingsMenuTexture{ nullptr };
		GameObject* SettingsMenuBack{ nullptr };
		GameObject* SettingsMenuFullscreen{ nullptr };

		//pointer to the game objects in the scene
		GameObject* SettingsMenuMV{ nullptr };
		GameObject* SettingsMenuMVCollider{ nullptr };

		//pointer to the game objects in the scene
		GameObject* SettingsMenuSFX{ nullptr };
		GameObject* SettingsMenuSFXCollider{ nullptr };

		//pointer to the game objects in the scene
		GameObject* SettingsMenuMusic{ nullptr };
		GameObject* SettingsMenuMusicCollider{ nullptr };

		//pointer to the pause menu
		GameObject* PauseMenu{ nullptr };

		//for collision checking
		CPCollider* coll{ nullptr };

	public:
		static float MVSliverPos;
		static float SFXSliderPos;
		static float MusicSliderPos;
		float width, half_width, collider_left;
	};

	REGISTER_SCRIPT(CPPSSettings)
}