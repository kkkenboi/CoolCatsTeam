/*!************************************************************************
 \file				CPPBehaviour.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				29/10/2023
 \brief
 This file contains the base class that all CPPScripts need to inherit
 from. It contains functions that all CPP scripts need to implement.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/Collisions.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	 CPPBehaviour is the base class for all CPP scripts, every script must have
	 a Start, Update, and Destroy function.
	*************************************************************************/
	class CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		 Defaults the destructor of CPPBehaviour
		*************************************************************************/
		virtual ~CPPBehaviour() = default;

		/*!***********************************************************************
		\brief
		 Called when the GameObject is created or when the scene starts
		*************************************************************************/
		virtual void Start() = 0;

		/*!***********************************************************************
		\brief
		 Called every frame when the game is running
		*************************************************************************/
		virtual void Update() = 0;

		/*!***********************************************************************
		\brief
		 Called when the GameObject is destroyed (E.g. when the scene stops)
		*************************************************************************/
		virtual void Destroy() = 0;

		/*!***********************************************************************
		\brief
		 Called when a collision happens with this gameobject, empty by design.
		 If something needs to happen, the script with override this function.
		*************************************************************************/
		virtual void OnCollisionEnter(CollisionData colData) 
		{
			UNREFERENCED_PARAMETER(colData);
		}

		GameObject* GameObj{nullptr};
	};


}
