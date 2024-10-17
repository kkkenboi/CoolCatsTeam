/*!************************************************************************
 \file				ScriptComponent.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin.chua@digipen.edu
 \par Course:       CSD2401A
 \date				23/10/2023
 \brief

 This file contains

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Components/ScriptComponent.h"
#include "LitterBox/GameLogic/GameLogic.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 Gets and stores the object instance from GameLogic
	*************************************************************************/
	void CPScript::Initialise()
	{
		GAMELOGIC->Load(this);
	}

	/*!***********************************************************************
	\brief
	 Calls the script's Update function
	*************************************************************************/
	void CPScript::Update()
	{
		// TODO: Link update to Mono
		//mono_runtime_invoke(m_classUpdate, )
	}

	/*!***********************************************************************
	\brief
	 Calls the script's Destroy function
	*************************************************************************/
	void CPScript::Destroy()
	{
		delete this;
	}
}
