/*!************************************************************************
 \file				ScriptComponent.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin.chua@digipen.edu
 \par Course:       CSD2401A
 \date				23/10/2023
 \brief

 This file contains functions of the CPScript class.

 CPScriptCPP is a component that stores a CS script that implements the
 game logic for this GameObject.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Component.h"

namespace LB 
{
	/*!***********************************************************************
	\brief
	 CPScript is a component to that holds a CS "script" class that holds the
	 game logic for this GameObject.
	*************************************************************************/
	class CPScript : public IComponent
	{
	public:
		/*!***********************************************************************
		\brief
		 Gets and stores the object instance from GameLogic
		*************************************************************************/
		void Initialise() override;

		/*!***********************************************************************
		\brief
		 Calls the script's Update function
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		 Calls the script's Destroy function
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		 Returns the type for this component
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPScript;
		}

	private:
	};
}
