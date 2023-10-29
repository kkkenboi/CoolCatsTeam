/*!************************************************************************
 \file				CPPScriptComponent.h
 \author(s)
 \par DP email(s):
 \par Course:       CSD2401A
 \date				29/10/2023
 \brief

 This file contains

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Component.h"

namespace LB
{
	/*!***********************************************************************
	\brief

	*************************************************************************/
	class CPScriptCPP : public IComponent
	{
	public:
		void Initialise() override;

		void Update() override;

		void Destroy() override;

		static ComponentTypeID GetType()
		{
			return C_CPScriptCPP;
		}

	private:
		void* m_instance{ nullptr };
	};
}
