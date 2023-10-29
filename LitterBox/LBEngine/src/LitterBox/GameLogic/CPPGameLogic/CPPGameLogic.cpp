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
	}

	void CPPGameLogic::Load(CPScriptCPP* newScript)
	{
		//--------------------LOADING OF SCRIPT BEHAVIOUR--------------------
		// Very basic RTTR for now
		if (newScript->GetName() == "Player")
		{

		}
		else
		{
			DebuggerLogErrorFormat("Tried to load invalid CPP Script %s.", newScript->GetName().c_str());
		}
		//--------------------LOADING OF SCRIPT BEHAVIOUR--------------------

		m_sceneScripts.push_back(newScript);
	}

	void CPPGameLogic::Unload(CPScriptCPP* scriptToRemove)
	{
		m_sceneScripts.remove(scriptToRemove);
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

	}
	//-------------------------CPP GAME LOGIC MANAGER-------------------------

	//--------------------------CPP SCRIPT COMPONENT--------------------------
	void CPScriptCPP::Initialise()
	{
		CPPGAMELOGIC->Load(this);
	}
}
