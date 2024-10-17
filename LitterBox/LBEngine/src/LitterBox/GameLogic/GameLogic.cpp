/*!************************************************************************
 \file				GameLogic.cpp
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

#include "pch.h"
#include "GameLogic.h"

#include "LitterBox/Debugging/Memory.h"

#include <filesystem>

namespace LB
{
	GameLogic* GAMELOGIC = nullptr;

	/*!***********************************************************************
	 \brief
	 Initalises the GameLogic system
	*************************************************************************/
	void GameLogic::Initialize()
	{
		// Ensure singleton
		if (!GAMELOGIC)
			GAMELOGIC = this;
		else
			DebuggerLogError("Game Logic System already exists!");

		SetSystemName("Game Logic System");

		// Find the mscorlib.dll in the ./Editor/mono/4.5 sub folder
	}

	/*!***********************************************************************
	 \brief
	 Adds a new script into the GameLogic system
	*************************************************************************/
	void GameLogic::Load(CPScript *newScript)
	{
		m_sceneScripts.push_back(newScript);
	}

	/*!***********************************************************************
	 \brief
	 Removes all scripts from the GameLogic system
	*************************************************************************/
	void GameLogic::Unload()
	{
		for (CPScript* script : m_sceneScripts)
		{
			script->Destroy();
		}

		m_sceneScripts.clear();
	}

	/*!***********************************************************************
	 \brief
	 Updates the script components for all GameObjects
	*************************************************************************/
	void GameLogic::Update()
	{
		for (CPScript* script : m_sceneScripts)
		{
			script->Update();
		}
	}

	/*!***********************************************************************
	 \brief
	 Destroys the GameLogic system

	 \return
	 Nothing
	*************************************************************************/
	void GameLogic::Destroy()
	{
		Unload();
	}


}
