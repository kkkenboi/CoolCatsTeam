/*!************************************************************************
 \file				CPPGameLogic.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				29/10/2023
 \brief

 This file contains 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Core/System.h"		// For ISystem
#include "LitterBox/Components/CPPScriptComponent.h"

namespace LB
{
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

		void Load(CPScriptCPP* newScript);

		void Unload(CPScriptCPP* scriptToRemove);

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
		std::list<CPScriptCPP*> m_sceneScripts;	// List of all scripts currently active in the scene
	};

	/*!***********************************************************************
	 \brief
	 A global pointer to our game so that it can be accessed from anywhere.
	*************************************************************************/
	extern CPPGameLogic* CPPGAMELOGIC;
}
