/*!************************************************************************
 \file				PauseQuitButton.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				26-11-2023
 \brief
 This file contains the functionality of the back button for the how to play
 screen during gameplay

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class PauseQuitButton : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPCollider* GetCollider();
	private:
		CPCollider* mCollider;

		GameObject* ConfirmMenuTexture;
		GameObject* ConfirmMenuYesButton;
		GameObject* ConfirmMenuNoButton;

		GameObject* PauseMenuTexture;
		GameObject* PauseMenuResumeButton;
		GameObject* PauseMenuHowToPlayButton;
		GameObject* PauseMenuExitButton;
	};

}