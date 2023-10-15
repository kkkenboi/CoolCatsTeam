/*!************************************************************************
 \file				GameLogic.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions declarations of the GameLogic class which is to
 handle all of the game logic in the system, eg. Health, Damage etc.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Core/System.h"
#include "mono/utils/mono-forward.h"

namespace LB
{
	//class MonoDomain;

	/*!***********************************************************************
	 \brief
	 GameLogic class will contain functions that checks whether GameObjects
	 should be active or nonactive after certain interactions and updates their
	 ingame stats as well
	*************************************************************************/
	class GameLogic : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initalises the Game Logic system
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 This should update any variables of all of the different GameObjects
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroys the GameLogic system
		*************************************************************************/
		void Destroy() override;

	private:
		MonoDomain *m_domain;
	};

	/*!***********************************************************************
	 \brief
	 A global pointer to our game so that it can be accessed from anywhere.

	*************************************************************************/
	extern GameLogic* GAMELOGIC;
}
