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

#include "mono/metadata/image.h"
#include "mono/jit/jit.h"

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

		mono_set_assemblies_path("Editor"); // Find the mscorlib.dll in the ./Editor/mono/4.5 sub folder

		m_domain = mono_jit_init("LitterBoxEngine");
		DebuggerAssert(m_domain, "[Mono] LitterBoxEngine domain could not be created!");

		m_scriptAssembly = mono_domain_assembly_open(m_domain, "CSharpAssembly.dll");
		DebuggerAssert(m_scriptAssembly, "[Mono] CSharpAssembly.dll not found! Please compile the dll first.");

		CPScript* test = DBG_NEW CPScript();
		test->Initialise();
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
		mono_jit_cleanup(m_domain);
	}

	/*!***********************************************************************
	 \brief
	 Returns the Mono domain which contains the assembly for the CS classes
	*************************************************************************/
	MonoDomain* GameLogic::GetDomain()
	{
		return m_domain;
	}

	/*!***********************************************************************
	 \brief
	 Returns the Assembly which contains all the CS classes
	*************************************************************************/
	MonoAssembly* GameLogic::GetScriptAssembly()
	{
		return m_scriptAssembly;
	}

}
