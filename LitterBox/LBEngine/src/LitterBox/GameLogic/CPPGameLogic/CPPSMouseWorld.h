/*!************************************************************************
 \file				CPPSMouseWorld.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				06-03-2024
 \brief
 This file contains the CPPSMouseWorld class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "pch.h"
#include "CPPBehaviour.h"
#include "LitterBox/Components/CPPScriptComponent.h"


namespace LB
{
	/*!***********************************************************************
	\brief

	*************************************************************************/
	class CPPSMouseWorld : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Overriden start function because of inheritance
		*************************************************************************/
		void Start() override;
		/*!***********************************************************************
		\brief
		Overriden update function because of inheritance
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		Overriden destroy function because of inheritance
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		This mainly checks for the collision between the mouse and the World to display
		pop up information upon entering.
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);

		/*!***********************************************************************
		\brief
		This mainly checks for the collision between the mouse and the World to display
		pop up information upon exiting.
		*************************************************************************/
		void OnCollisionExit(CollisionData colData) override;

	private:
		GameObject* m_PlayerHUD{ nullptr };
		GameObject* m_UpgradeObject{ nullptr };
	};
	REGISTER_SCRIPT(CPPSMouseWorld)
}