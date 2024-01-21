/*!************************************************************************
 \file				CPPScriptComponent.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				29/10/2023
 \brief
 This file contains functions of the CPScriptCPP class.

 CPScriptCPP is a component that stores a CPP script that implements the
 game logic for this GameObject.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPScriptComponent.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPGameLogic.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 Loads the name of the script for creation
	*************************************************************************/
	bool CPScriptCPP::Deserialize(const Value& data)
	{
		bool HasScript = data.HasMember("Script");
		if (data.IsObject())
		{
			if (HasScript)
			{
				const Value& nameValue = data["Script"];
				m_type = CPPGameLogic::Instance()->GetTypeRegistry()[nameValue.GetString()].m_type;
				return true;
			}
		}
		return false;
	}
}