/*!************************************************************************
 \file				CPPSBlueMushroom.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
  This file contains the behviour of the Blue Mushroom obstacle.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSBlueMushroom : public CPPBehaviour
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
		Gets the object's render component
		*************************************************************************/
		CPRender* GetRender();
		/*!***********************************************************************
		\brief
		Gets the object's rigidbody component
		*************************************************************************/
		CPRigidBody* GetRigidBody();
		/*!***********************************************************************
		\brief
		Gets the objcet's collider component
		*************************************************************************/
		CPCollider* GetCollider();

		/*!***********************************************************************
		\brief
		Every time the mushroom collides with anything it's scale changes
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);
	private:
		CPRender* mRender{ nullptr };
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };
		GameObject* mPlayer{ nullptr };

		float mScaleTimer{ 0.35f };
		float mScaleTimerRemaining{ 0.35f };
		bool mScaled{ false };
	};

	REGISTER_SCRIPT(CPPSBlueMushroom)
}