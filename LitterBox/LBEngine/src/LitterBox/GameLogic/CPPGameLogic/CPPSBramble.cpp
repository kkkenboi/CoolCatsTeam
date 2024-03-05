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

#include "CPPSBramble.h"
#include "LitterBox/Engine/Time.h"


namespace LB
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSBramble::Start()
	{
		//mTransform = GameObj->GetComponent<CPTransform>();
		//mRender = GameObj->GetComponent<CPRender>();
		//mRigidBody = GameObj->GetComponent<CPRigidBody>();
		//mCollider = GameObj->GetComponent<CPCollider>();
		//mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
	}

	/*!***********************************************************************
	\brief
	Update function where the mushroom's scale is being changed
	*************************************************************************/
	void CPPSBramble::Update()
	{
		OnInteraction(0, 1);
	}

	/*!***********************************************************************
	\brief
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSBramble::Destroy()
	{

	}
}