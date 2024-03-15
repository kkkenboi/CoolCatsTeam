/*!************************************************************************
 \file                CPPSDummy.h
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                15-03-2024
 \brief

	This file contains the dummy enemy

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPSBaseEnemy.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB
{
	class CPPSDummy : public CPPSBaseEnemy
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;
		void Hurt() override;

		void OnCollisionEnter(CollisionData colData) override;
		void Die() override;
	};
	REGISTER_SCRIPT(CPPSDummy)
}