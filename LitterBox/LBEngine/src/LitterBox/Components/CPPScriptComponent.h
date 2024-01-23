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

#pragma once

#include "Component.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPGameLogic.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPBehaviour.h"

#include <typeindex>

namespace LB
{
	/*!***********************************************************************
	\brief
	 CPScriptCPP is a component to that holds a CPP "script" class that holds the
	 game logic for this GameObject.
	*************************************************************************/
	class CPScriptCPP : public IComponent
	{
	public:
		/*!***********************************************************************
		\brief
		 Defaults the destructor of the CPScriptCPP class
		*************************************************************************/
		virtual ~CPScriptCPP() = default;

		/*!***********************************************************************
		\brief
		 Gets and stores the object instance from CPPScriptManager 
		*************************************************************************/
		void Initialise() override;

		/*!***********************************************************************
		\brief
		 Calls the script's Start function
		*************************************************************************/
		void Start()
		{
			if (m_instance)
				m_instance->Start();
		}

		/*!***********************************************************************
		\brief
		 Calls the script's Update function
		*************************************************************************/
		void Update() override
		{
			if (m_instance)
				m_instance->Update();
		}

		/*!***********************************************************************
		\brief
		 Calls the script's destroy function and deletes the object instance
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		 Returns the name of the script (not file name)
		*************************************************************************/
		std::type_index GetScriptType() const
		{
			return m_type;
		}

		/*!***********************************************************************
		\brief
		 Sets the name of the script (not file name)
		*************************************************************************/
		void SetScriptType(std::type_index const& newType)
		{
			m_type = newType;
		}

		/*!***********************************************************************
		\brief
		 Stores the pointer to the script object and set's the gameObj reference
		 for the script to use
		*************************************************************************/
		void SetInstance(CPPBehaviour* newScript)
		{
			m_instance = newScript;
			m_instance->GameObj = gameObj;
		}

		/*!***********************************************************************
		\brief
		 Saves the name of the script
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{

			data.SetObject();
			Value scriptName(m_type.name(), alloc);
			data.AddMember("Script", scriptName, alloc);
			return true;
		}

		/*!***********************************************************************
		\brief
		 Loads the name of the script for creation
		*************************************************************************/
		bool Deserialize(const Value& data) override;

		/*!***********************************************************************
		\brief
		 Returns the type for this component
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPScriptCPP;
		}

		/*!***********************************************************************
		\brief
		 Returns the current script that is being used
		*************************************************************************/
		CPPBehaviour* GetInstance() 
		{
			return m_instance;
		}

	private:
		std::type_index m_type{ typeid(void) };
		CPPBehaviour* m_instance{ nullptr };	// Pointer to the script object
	};
}
