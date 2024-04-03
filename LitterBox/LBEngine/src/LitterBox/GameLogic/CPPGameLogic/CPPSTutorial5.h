#pragma once
/*!************************************************************************
 \file                CPPSTutorial5.h
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                15-03-2024
 \brief

	This file contains the logic for the fifth tutorial message

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSTutorial5 : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		/*!***********************************************************************
		\brief
		Changes the collided variable to true when the MainChar has collided with it
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData) override;

		bool collided{ false };
	};
	REGISTER_SCRIPT(CPPSTutorial5)
}