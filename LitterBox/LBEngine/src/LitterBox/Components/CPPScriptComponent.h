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
			if (m_instance)
				m_instance->Start();
		}

		void Update() override
		{
			m_instance->Update();
		}

		void Destroy() override;

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
			m_instance->GameObj = gameObj;
		}

		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{

			data.SetObject();
			Value scriptName(m_name.c_str(), alloc);
			data.AddMember("Script", scriptName, alloc);
			return true;
		}

		bool Deserialize(const Value& data) override
		{
			bool HasScript = data.HasMember("Script");
			if (data.IsObject())
			{
				if (HasScript)
				{
					const Value& nameValue = data["Script"];
					m_name = nameValue.GetString();
					return true;
				}
			}
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
