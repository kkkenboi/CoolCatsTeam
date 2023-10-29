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
#include "LitterBox/GameLogic/CPPGameLogic/CPPGameLogic.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPBehaviour.h"

namespace LB
{
	/*!***********************************************************************
	\brief

	*************************************************************************/
	class CPScriptCPP : public IComponent
	{
	public:
		void Initialise() override;

		void Start()
		{
			m_instance->Start();
		}

		void Update() override
		{
			m_instance->Update();
		}

		void Destroy() override
		{
			m_instance->Destroy();
		}

		std::string const& GetName() const
		{
			return m_name;
		}

		void SetName(std::string const& newName)
		{
			m_name = newName;
		}

		void SetInstance(CPPBehaviour* newScript)
		{
			m_instance = newScript;
		}

		bool Serialize(Value & data, Document::AllocatorType & alloc) override
		{
			return false;
		}

		bool Deserialize(const Value& data) override
		{
			return false;
		}

		static ComponentTypeID GetType()
		{
			return C_CPScriptCPP;
		}

	private:
		std::string m_name{ "CPPScript" };
		CPPBehaviour* m_instance{ nullptr };
	};
}
