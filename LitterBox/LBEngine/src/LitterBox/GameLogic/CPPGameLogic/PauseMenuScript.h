/*!************************************************************************
 \file				PauseMenuScript.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
  This file contains the functionality of the PauseMenuScript

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseMenuScript : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		 Initializes the GameObjects needed for the script
		*************************************************************************/
		void Start() override;
		/*!***********************************************************************
		\brief
		 Allows for button press and movement of other game objects into view
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		 Override destroy function
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		 Getter for the OnPauseMenu member variable

		\return
		 OnPauseMenu member variable
		*************************************************************************/
		bool& GetOnPauseMenu();

	private:
		bool OnPauseMenu{false};
		bool BackButtonHovered{ false };
		bool BackToMainMenu{ false };
		bool HowToPlayOrSettingMode{ false };

		// Pause Menu
		GameObject* MenuTexture{ nullptr };
		GameObject* ResumeButton{ nullptr };
		GameObject* HowToPlayButton{ nullptr };
		GameObject* QuitButton{ nullptr };

		GameObject* Settings{ nullptr };
		GameObject* MainMenu{ nullptr };
		GameObject* BackToMainMenuTexture { nullptr };
		// Title 
		GameObject* Title{ nullptr };

		// Mouse
		GameObject* MouseCursor{ nullptr };

		// How to play
		GameObject* HowToPlayTexture{ nullptr };
		GameObject* HowToPlayBackButton{ nullptr };

		// Quit
		GameObject* QuitTexture{ nullptr };
		GameObject* QuitYesButton{ nullptr };
		GameObject* QuitNoButton{ nullptr };

		// Settings
		GameObject* SettingsMenuTexture{ nullptr };
		GameObject* SettingsMenuBack{ nullptr };
		GameObject* SettingsMenuFullscreen{ nullptr };

		GameObject* SettingsMenuMV{ nullptr };
		GameObject* SettingsMenuMVCollider{ nullptr };

		GameObject* SettingsMenuSFX{ nullptr };
		GameObject* SettingsMenuSFXCollider{ nullptr };

		GameObject* SettingsMenuMusic{ nullptr };
		GameObject* SettingsMenuMusicCollider{ nullptr };

		// Underline
		GameObject* Underline{nullptr};
	};
	REGISTER_SCRIPT(PauseMenuScript)
}