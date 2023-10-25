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

#include "LitterBox/Components/ScriptComponent.h"
#include "LitterBox/GameLogic/GameLogic.h"

namespace LB
{
	/*!***********************************************************************
	\brief

	*************************************************************************/
	void CPScript::Initialise()
	{
		GAMELOGIC->Load(this);

		//MonoAssembly* scriptAssembly = GAMELOGIC->GetScriptAssembly();
		//m_classUpdate = mono_method_desc_new("Test::Update", false);
		//mono_class_from_name(scriptAssembly, "LB", "TestBehavior");
	}

	void CPScript::Update()
	{
		//mono_runtime_invoke(m_classUpdate, )
	}

	void CPScript::Destroy()
	{

	}

	ComponentTypeID CPScript::GetType()
	{
		return C_CPScript;
	}
}
