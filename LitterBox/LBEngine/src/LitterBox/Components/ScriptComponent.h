/*!************************************************************************
 \file				ScriptComponent.h
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

#pragma once

#include "Component.h"
#include "mono/metadata/debug-helpers.h"	// For Mono_method_desc

namespace LB 
{
	/*!***********************************************************************
	\brief
	
	*************************************************************************/
	class CPScript : public IComponent
	{
	public:
		void Initialise() override;

		void Update() override;

		void Destroy() override;

		ComponentTypeID GetType() override;

	private:

		MonoClass *m_class;
		MonoMethodDesc *m_classUpdate;
	};
}
