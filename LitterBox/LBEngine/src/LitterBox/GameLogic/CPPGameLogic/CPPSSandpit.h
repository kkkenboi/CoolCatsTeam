/*!************************************************************************
 \file                CPPSSandpit.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                15-03-2024
 \brief

	This file contains the logic for the sandpit obstacle in game

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

#pragma once
namespace LB
{
	class CPPSSandpit : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Start function where variables will be initialised
		*************************************************************************/
		void Start() override;
		/*!***********************************************************************
		\brief
		Update function where the mushroom's scale is being changed
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		Overriden destroy function because of inheritance
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		Every time the mushroom collides with anything it's scale changes
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);

		/*!***********************************************************************
		\brief
		Happens every time its currently colliding with another object
		*************************************************************************/
		void OnCollisionStay(CollisionData colData);

		/*!***********************************************************************
		\brief
		Happens every time an object stops colliding with the sandpit
		*************************************************************************/
		void OnCollisionExit(CollisionData colData);
	private:
		GameObject* mPlayer{ nullptr };
	};

	REGISTER_SCRIPT(CPPSSandpit)
}