/*!************************************************************************
 \file				GameLogic.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin.chua@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief
 This file handles the initialization of C# Mono which all script components
 will use.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Core/System.h"		// For ISystem
#include "LitterBox/Components/ScriptComponent.h"

namespace LB
{
	class CPScript;

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
		 Initalises the GameLogic system
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 Adds a new script into the GameLogic system
		*************************************************************************/
		void Load(CPScript* newScript);

		/*!***********************************************************************
		 \brief
		 Removes all scripts from the GameLogic system
		*************************************************************************/
		void Unload();

		/*!***********************************************************************
		 \brief
		 Updates the script components for all GameObjects
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroys the GameLogic system
		*************************************************************************/
		void Destroy() override;


	private:
		std::list<CPScript*> m_sceneScripts;	// List of all scripts currently active in the scene
	};

	/*!***********************************************************************
	 \brief
	 A global pointer to our game so that it can be accessed from anywhere.
	*************************************************************************/
	extern GameLogic* GAMELOGIC;
}
