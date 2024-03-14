/*!************************************************************************
 \file				CPPSBush.h
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2451A
 \date				10-03-2024
 \brief
  This file contains the behviour of the Bush obstacle.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "CPPSBaseBouncy.h"

namespace LB
{
	class CPPSBush : public CPPSBaseBouncy
	{
	public:
		/*!***********************************************************************
		\brief
		Start function where variables will be initialised
		*************************************************************************/
		void Start() override;

		/*!***********************************************************************
		\brief
		Update function where the bush's scale is being changed
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		Overriden destroy function because of inheritance
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		\brief
		Overriden OnCollisionEnter to prevent velocity changing
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);
	};

	REGISTER_SCRIPT(CPPSBush)
}
