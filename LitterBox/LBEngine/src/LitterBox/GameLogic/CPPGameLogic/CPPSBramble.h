/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2451A
 \date
 \brief
  This file contains

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "CPPSBaseBouncy.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSBramble : public CPPSBaseBouncy
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
	};

	REGISTER_SCRIPT(CPPSBramble)
}