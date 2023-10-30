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
#include "mono/jit/jit.h"							// For mono_object_new
#include "mono/metadata/assembly.h"					// For mono_assembly_get_image

namespace LB
{
	/*!***********************************************************************
	\brief

	*************************************************************************/
	void CPScript::Initialise()
	{
		GAMELOGIC->Load(this);

		//MonoImage* image = mono_assembly_get_image(GAMELOGIC->GetScriptAssembly());
		//DebuggerLogWarningFormat("Image Exists! %d", image != nullptr);

		//MonoClass* klass = mono_class_from_name(image, "", "Player");
		//DebuggerLogWarningFormat("Class Exists! %d", klass != nullptr);

		//m_instance = mono_object_new(GAMELOGIC->GetDomain(), klass);
		//DebuggerLogWarningFormat("Instance Exists! %d", m_instance != nullptr);

		//MonoMethod* method = mono_class_get_method_from_name(klass, "Update", 0);
		//DebuggerLogWarningFormat("Method Exists! %d", method != nullptr);

		//mono_runtime_invoke(method, m_instance, nullptr, nullptr);
	}

	void CPScript::Update()
	{
		//mono_runtime_invoke(m_classUpdate, )
	}

	void CPScript::Destroy()
	{
		delete this;
	}
}
