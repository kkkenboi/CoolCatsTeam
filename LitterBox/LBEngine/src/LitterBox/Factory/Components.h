/*!************************************************************************
 \file				Components.h
 \author(s)			Kenji Brannon Chong, Ang Jiawei Jarrett
 \par DP email(s):	kenjibrannon.c@digipen.edu, a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 This file contains the header for all components and the ComponentMaker 
 and ComponentMakerType class.

 If you want access to all components, include this Components header! 
 Otherwise, if you need a specific component, include the respective header.
					
 ComponentMaker acts as the interface for different ComponentMakerType 
 classes to create ComponentMaker of those types. ComponentMakers are 
 incharge to create components of that type.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
// Put all the component header files here
#include "LitterBox/Components/TransformComponent.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Components/RigidBodyComponent.h"
#include "LitterBox/Components/ColliderComponent.h"
#include "LitterBox/Components/CameraComponent.h"
#include "LitterBox/Components/ScriptComponent.h"
#include "LitterBox/Components/CPPScriptComponent.h"
#include "LitterBox/Components/TextComponent.h"
#include "LitterBox/Components/AudioSourceComponent.h"
#include "LitterBox/Components/AnimatorComponent.h"
#include "LitterBox/Components/ParticleComponent.h"
#include "LitterBox/Audio/AudioListenerComponent.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Interface to make components and tag IDs
	*************************************************************************/
	class ComponentMaker
	{
	public:
		/*!***********************************************************************
		 \brief
		 Constructor, what type of component to create
		*************************************************************************/
		ComponentMaker(ComponentTypeID typeId) : TypeID(typeId) {}

		/*!***********************************************************************
		 \brief
		 Creates the component based on the typeID
		*************************************************************************/
		virtual IComponent* Create() = 0;

		/*!***********************************************************************
		 \brief
		 Virtual destructor
		*************************************************************************/
		virtual ~ComponentMaker() {};

		ComponentTypeID TypeID;
	};

	/*!***********************************************************************
	 \brief
	 Class Template to create different types of ComponentMaker
	 ComponentType here refers to the derived classes of IComponent
	*************************************************************************/
	template<typename ComponentType>
	class ComponentMakerType : public ComponentMaker
	{
	public:
		/*!***********************************************************************
		 \brief
		 Child constructor from component maker
		*************************************************************************/
		ComponentMakerType(ComponentTypeID typeId) : ComponentMaker(typeId) {}

		/*!***********************************************************************
		 \brief
		 Creates an instance of the component based on the typeID
		*************************************************************************/
		IComponent* Create() override
		{
			// Returns a constructor to the derived class of IComponent
			return Memory::Instance()->Allocate<ComponentType>();
		}
	};

}

// Marco for defining different makers for componentss
#define CreateComponentMaker(ComponentType, ComponentTypeID) FACTORY->InitCM (ComponentTypeID, DBG_NEW ComponentMakerType<ComponentType>( ComponentTypeID ) );