/*!************************************************************************
 \file				CPPGameLogic.cpp
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

#include "pch.h"
#include "CPPGameLogic.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Engine/Time.h"

// Include scripts here
#include "CPPSPlayer.h"

namespace LB
{
	CPPGameLogic* CPPGAMELOGIC = nullptr;

	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	/*!***********************************************************************
	 \brief
	 Initalises the Game Logic system

	 \return
	 Nothing
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

	void CPPGameLogic::Load(CPScriptCPP* newScript)
	{
		DebuggerLogWarning("Added script");

		if (newScript->GetName() != "Player")
		{
			DebuggerLogWarningFormat("Tried to load invalid CPP Script %s.", newScript->GetName().c_str());
			return;
		}

		m_sceneScripts.push_back(newScript);
	}

	void CPPGameLogic::Unload(CPScriptCPP* scriptToRemove)
	{
		m_sceneScripts.remove(scriptToRemove);
	}

	void CPPGameLogic::Start()
	{
		for (CPScriptCPP* script : m_sceneScripts)
		{
			//--------------------LOADING OF SCRIPT BEHAVIOUR--------------------
			// Very basic RTTR for now
			if (script->GetName() == "Player")
			{
				script->SetInstance(new CPPSPlayer);
			}
			else if (script->GetName() == "Enemy") {

			}
			//--------------------LOADING OF SCRIPT BEHAVIOUR--------------------

			script->Start();
		}
	}

	// For event subscription
	void StartScripts(bool isPlaying)
	{
		if (isPlaying)
			CPPGAMELOGIC->Start();
	}

	/*!***********************************************************************
	 \brief
	 This should update any variables of all of the different GameObjects

	 \return
	 Nothing
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
	 Destroys the GameLogic system

	 \return
	 Nothing
	*************************************************************************/
	void CPPGameLogic::Destroy()
	{
		m_sceneScripts.clear();
	}
	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	//--------------------------CPP SCRIPT COMPONENT--------------------------
	void CPScriptCPP::Initialise()
	{
		CPPGAMELOGIC->Load(this);
	}

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
}
