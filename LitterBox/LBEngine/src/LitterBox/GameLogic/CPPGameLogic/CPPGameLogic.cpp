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

// Include scripts here
#include "CPPSPlayer.h"
#include "Butt.h"
#include "CPPSChaser.h"
#include "QuitScript.h"
#include "CPPSMage.h"
#include "PauseQuitButton.h"

namespace LB
{
	CPPGameLogic* CPPGAMELOGIC = nullptr;
	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	/*!***********************************************************************
	 \brief
	 Initalises the Game Logic system
	*************************************************************************/
	void CPPGameLogic::Initialize()
	{
		// Ensure singleton
		if (!CPPGAMELOGIC)
			CPPGAMELOGIC = this;
		else
			DebuggerLogError("CPP Game Logic System already exists!");

		SetSystemName("CPP Game Logic System");

		CORE->onPlayingModeToggle.Subscribe(StartScripts);
	}

	/*!***********************************************************************
	 \brief
	 Adds a new script to the list of scripts to load when scene plays
	*************************************************************************/
	void CPPGameLogic::Load(CPScriptCPP* newScript)
	{
		if (newScript->GetName() == "Player" || 
			newScript->GetName() == "Enemy" || 
			newScript->GetName() == "Butt" ||
			newScript->GetName() == "Quit" ||
			newScript->GetName() == "Mage" ||
			newScript->GetName() == "PauseQuit")
		{
			m_sceneScripts.push_back(newScript);

			// If scene is already running, start the script immediately
			if (CORE->IsPlaying())
			{
				StartScript(newScript);
			}

			return;
		}

		DebuggerLogWarningFormat("Tried to load invalid CPP Script %s.", newScript->GetName().c_str());
	}

	/*!***********************************************************************
	 \brief
	 Removes the given script from the list of scripts
	*************************************************************************/
	void CPPGameLogic::Unload(CPScriptCPP* scriptToRemove)
	{
		m_sceneScripts.remove(scriptToRemove);
	}

	/*!***********************************************************************
	 \brief
	 Creates and loads the instances of each script when the scene starts
	*************************************************************************/
	void CPPGameLogic::Start()
	{
		for (CPScriptCPP* script : m_sceneScripts)
		{
			StartScript(script);
			script->Start();
		}
	}

	void CPPGameLogic::StartScript(CPScriptCPP* script)
	{
			// If script already has an instance, ignore!
			if (script->GetInstance()) return;

			//--------------------LOADING OF SCRIPT BEHAVIOUR--------------------
			// Very basic RTTR for now
			if (script->GetName() == "Player")
			{
				script->SetInstance(DBG_NEW CPPSPlayer);
			}
			else if (script->GetName() == "Enemy") {
				script->SetInstance(DBG_NEW CPPSChaser);
			}
			else if (script->GetName() == "Butt") {
				script->SetInstance(DBG_NEW Butt);
			}
			else if (script->GetName() == "Quit") {
				script->SetInstance(DBG_NEW QuitScript);
			}
			else if (script->GetName() == "Mage") {
				script->SetInstance(DBG_NEW CPPSMage);
			}
			else if (script->GetName() == "PauseQuit") {
				script->SetInstance(DBG_NEW PauseQuitButton);
			}
			//--------------------LOADING OF SCRIPT BEHAVIOUR--------------------
	}

	/*!***********************************************************************
	 \brief
	 For event subscription for when the scene starts
	*************************************************************************/
	void StartScripts(bool isPlaying)
	{
		if (isPlaying)
			CPPGAMELOGIC->Start();
	}

	/*!***********************************************************************
	 \brief
	 Updates the script components for all GameObjects
	*************************************************************************/
	void CPPGameLogic::Update()
	{
		if (!CORE->IsPlaying() || TIME->IsPaused()) return;

		for (CPScriptCPP* script : m_sceneScripts)
		{
			script->Update();
		}
	}

	/*!***********************************************************************
	 \brief
	 Destroys the GameLogic system and clears all scripts attached
	*************************************************************************/
	void CPPGameLogic::Destroy()
	{
		m_sceneScripts.clear();
	}
	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	//--------------------------CPP SCRIPT COMPONENT--------------------------

	/*!***********************************************************************
	\brief
		Gets and stores the object instance from CPPScriptManager
	*************************************************************************/
	void CPScriptCPP::Initialise()
	{
		CPPGAMELOGIC->Load(this);
	}

	/*!***********************************************************************
	\brief
	 Calls the script's destroy function and deletes the object instance
	*************************************************************************/
	void CPScriptCPP::Destroy()
	{
		if (m_instance)
		{
			m_instance->Destroy();
			delete m_instance;
			m_instance = nullptr;
		}
		CPPGAMELOGIC->Unload(this);
	}

	//--------------------------CPP SCRIPT COMPONENT--------------------------
}
