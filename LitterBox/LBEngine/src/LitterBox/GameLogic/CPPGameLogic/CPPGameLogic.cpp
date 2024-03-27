/*!************************************************************************
 \file				CPPGameLogic.cpp
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

#include "pch.h"
#include "CPPGameLogic.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Scene/SceneManager.h"

namespace LB
{
	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	/*!***********************************************************************
	 \brief
	 Initalises the Game Logic system
	*************************************************************************/
	void CPPGameLogic::Initialize()
	{
		SetSystemName("CPP Game Logic System");

		RegisterAll();

		CORE->onPlayingModeToggle.Subscribe(StartScripts);
	}

	/*!***********************************************************************
	 \brief
	 Adds a new script to the list of scripts to load when scene plays
	*************************************************************************/
	void CPPGameLogic::Load(CPScriptCPP* newScript)
	{
		m_sceneScripts.push_back(newScript);

		// If scene is already running, start the script immediately
		if (CORE->IsPlaying() && SCENEMANAGER->IsSceneFullyLoaded() && !m_sceneStarted)
		{
			StartScript(newScript);
			newScript->Start();
		}
	}

	/*!***********************************************************************
	 \brief
	 Removes the given script from the list of scripts
	*************************************************************************/
	void CPPGameLogic::Unload(CPScriptCPP* scriptToRemove)
	{
		auto script = std::find(m_sceneScripts.begin(), m_sceneScripts.end(), scriptToRemove);

		if (script != m_sceneScripts.end())
		{
			m_sceneScripts.erase(script);
		}
	}

	/*!***********************************************************************
	 \brief
	 Creates and loads the instances of each script when the scene starts
	*************************************************************************/
	void CPPGameLogic::Start()
	{
		DebuggerLog("Start for CPPGameLogic called");

		m_sceneStarted = true;

		for (int index{ 0 }; index < m_sceneScripts.size(); ++index)
		{
			StartScript(m_sceneScripts[index]);
			m_sceneScripts[index]->Start();
		}

		m_sceneStarted = false;
	}

	/*!***********************************************************************
	 \brief
	 Returns the registry containing all the scripts stored by the
	 GameLogic manager.
	*************************************************************************/
	std::map<std::type_index, ScriptFactoryBase*>& CPPGameLogic::GetRegistry()
	{
		return m_scriptRegistry;
	}

	/*!***********************************************************************
	 \brief
	 Returns the registry containing all the script types stored by the
	 GameLogic manager.
	*************************************************************************/
	std::map<std::string, ScriptTypeID>& CPPGameLogic::GetTypeRegistry()
	{
		return m_scriptTypeRegistry;
	}

	/*!***********************************************************************
	 \brief
	 Loads the CPPScript instance based on the name of the script.
	*************************************************************************/
	void CPPGameLogic::StartScript(CPScriptCPP* script)
	{
			// If script already has an instance, ignore!
			if (script->GetInstance()) return;

			script->SetInstance(m_scriptRegistry[script->GetScriptType()]->CreateInstance());
	}

	/*!***********************************************************************
	 \brief
	 For event subscription for when the scene starts
	*************************************************************************/
	void StartScripts(bool isPlaying)
	{
		if (isPlaying)
		{
			CPPGameLogic::Instance()->Start();
		}
	}

	/*!***********************************************************************
	 \brief
	 Updates the script components for all GameObjects
	*************************************************************************/
	void CPPGameLogic::Update()
	{
		if (!CORE->IsPlaying()) return;

		for (int index{ 0 }; index < m_sceneScripts.size(); ++index)
		{
			CPScriptCPP* script = m_sceneScripts[index];
			// TODO: Refactor script deletion in loop
			if (script)
				script->Update();
		}
	}

	void CPPGameLogic::FixedUpdate()
	{
		if (!CORE->IsPlaying()) return;

		for (int index{ 0 }; index < m_sceneScripts.size(); ++index)
		{
			CPScriptCPP* script = m_sceneScripts[index];
			// TODO: Refactor script deletion in loop
			if (script)
				script->FixedUpdate();
		}
	}

	/*!***********************************************************************
	 \brief
	 Destroys the GameLogic system and clears all scripts attached
	*************************************************************************/
	void CPPGameLogic::Destroy()
	{
		m_sceneScripts.clear();

		for (auto it = m_scriptRegistry.begin(); it != m_scriptRegistry.end(); ++it)
		{
			delete it->second;
		}

		m_scriptRegistry.clear();
	}
	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	//--------------------------CPP SCRIPT COMPONENT--------------------------

	/*!***********************************************************************
	\brief
		Gets and stores the object instance from CPPScriptManager
	*************************************************************************/
	void CPScriptCPP::Initialise()
	{
		CPPGameLogic::Instance()->Load(this);
	}

	/*!***********************************************************************
	\brief
	 Calls the script's destroy function and deletes the object instance
	*************************************************************************/
	void CPScriptCPP::Destroy()
	{
		CPPGameLogic::Instance()->Unload(this);
		if (m_instance)
		{
			m_instance->Destroy();
			Memory::Instance()->Deallocate(m_instance);
			m_instance = nullptr;
		}
	}

	//--------------------------CPP SCRIPT COMPONENT--------------------------
}
