/*!************************************************************************
 \file				Component.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief
 This file contains functions of the IComponent class, as well as the enum
 to identify different components.

 IComponent acts as the interface for all GameObject components to
 build off from to create components to be attached to GameObjects. These
 derived components are then updated by their own systems.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Serialization/Serializer.h"
#include "ComponentTypeID.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 IComponent class provides an interface for other components to build off
	 from.
	*************************************************************************/
	class IComponent
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initialises the component
		*************************************************************************/
		virtual void Initialise() {}

		/*!***********************************************************************
		 \brief
		 Updates the component
		*************************************************************************/
		virtual void Update() {}

		/*!***********************************************************************
		 \brief
		 Serialises the components based on its derived member's data
		*************************************************************************/
		virtual bool Serialize(Value&, Document::AllocatorType&) { return false; }

		/*!***********************************************************************
		 \brief
		 Deserialises the components based on its derived member's data
		*************************************************************************/
		virtual bool Deserialize(const Value&) { return false; }

		/*!***********************************************************************
		 \brief
		 Destroys the component
		*************************************************************************/
		virtual void Destroy() {}

		/*!***********************************************************************
		 \brief
		 Toggles the ACTUAL active state for this component
		*************************************************************************/
		virtual void ToggleActive(bool isActive) { UNREFERENCED_PARAMETER(isActive); }

		/*!***********************************************************************
		 \brief
		 Toggles the FLAG for the active state for this component
		*************************************************************************/
		void ToggleActiveFlag(bool isActive)
		{
			m_active = isActive;
			if (m_active && gameObj->IsActive())
			{
				ToggleActive(true);
			}
			else
			{
				ToggleActive(false);
			}
		}

		/*!***********************************************************************
		 \brief
		 Virtual destructor in case any components has a destructor to call
		*************************************************************************/
		virtual ~IComponent() {}

		GameObject* gameObj{ nullptr }; // Every component has a reference to its object it is part of
		bool m_active{ true };
	};
}