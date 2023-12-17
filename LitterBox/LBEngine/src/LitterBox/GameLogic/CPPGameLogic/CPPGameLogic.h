/*!************************************************************************
 \file				CPPGameLogic.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				29/10/2023
 \brief
 This file handles the initialization of all CPP scripts loaded by
 GameObjects. Scripts only function when the game is playing

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Core/System.h"		// For ISystem
#include "LitterBox/Components/CPPScriptComponent.h"

namespace LB
{
	class CPScriptCPP;

	/*!***********************************************************************
	 \brief
	 GameLogic class will contain functions that checks whether GameObjects
	 should be active or nonactive after certain interactions and updates their
	 ingame stats as well
	*************************************************************************/
	class CPPGameLogic : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initalises the Game Logic system
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 Start for CPPGamelogic manager. Creates and loads the instances of each 
		 script when the scene starts
		*************************************************************************/
		void Start();

		/*!***********************************************************************
		 \brief
		 Loads the CPPScript instance based on the name of the script.
		*************************************************************************/
		void StartScript(CPScriptCPP* script);

		/*!***********************************************************************
		 \brief
		 Adds a new script to the list of scripts to load when scene plays
		*************************************************************************/
		void Load(CPScriptCPP* newScript);

		/*!***********************************************************************
		 \brief
		 Removes the given script from the list of scripts
		*************************************************************************/
		void Unload(CPScriptCPP* scriptToRemove);

		/*!***********************************************************************
		 \brief
		 Updates the script components for all GameObjects
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroys the GameLogic system and clears all scripts attached
		*************************************************************************/
		void Destroy() override;

	private:
		std::vector<CPScriptCPP*> m_sceneScripts{};	// List of all scripts currently active in the scene
	};

	/*!***********************************************************************
	 \brief
	 For event subscription for when the scene starts
	*************************************************************************/
	void StartScripts(bool isPlaying);

	/*!***********************************************************************
	 \brief
	 A global pointer to our game so that it can be accessed from anywhere.
	*************************************************************************/
	extern CPPGameLogic* CPPGAMELOGIC;
}
