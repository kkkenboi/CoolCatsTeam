/*!************************************************************************
 \file				ScriptComponent.cpp
 \author(s)
 \par DP email(s):
 \par Course:       CSD2401A
 \date				23/10/2023
 \brief

 This file contains

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "ScriptComponent.h"
#include "LitterBox/GameLogic/GameLogic.h"
#include "mono/metadata/class.h"

namespace LB
{
	void CPScript::Initialise()
	{
		GAMELOGIC->Load(this);
	}

	void CPScript::Update()
	{
		//mono_runtime_invoke(m_classUpdate, )
	}

	void CPScript::Destroy()
	{

	}
}