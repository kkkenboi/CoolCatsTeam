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

#include "CPPSBaseBouncy.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSBlueMushroom : public CPPSBaseBouncy
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

	private:
		//CPTransform* mTransform{ nullptr };
		//CPRender* mRender{ nullptr };
		//CPRigidBody* mRigidBody{ nullptr };
		//CPCollider* mCollider{ nullptr };
		//GameObject* mPlayer{ nullptr };

		//float mToMaxTimer{ 0.15f };
		//float mToMinTimer{ 0.35f };
		//float mScaleTimer{ 0.35f };
		//float mScaleTimerRemaining{ 0.35f };

		//Vec2<float> mScaleOG{ 1.f,1.f };
		//Vec2<float> mScaleMax{ 1.25f , 1.25f };


		//bool mScaledUp{ false };
		//bool mScaledDown{ true };
	};

	REGISTER_SCRIPT(CPPSBlueMushroom)
}