/*!************************************************************************
 \file                CPPSTutorial3.cpp
 \author(s)           Justine Carlo Villa Ilao
 \par DP email(s):    justine.c@digipen.edu
 \par Course:         CSD2401A
 \date                15-03-2024
 \brief

	This file contains the logic for the third tutorial message

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSTutorial4.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Inherited virtual base class function that needs to be overriden for Start
	*************************************************************************/
	void CPPSTutorial4::Start()
	{

	}
	/*!***********************************************************************
	\brief
	Inherited virtual base class function that needs to be overriden for Update
	*************************************************************************/
	void CPPSTutorial4::Update()
	{

	}
	/*!***********************************************************************
	\brief
	Inherited virtual base class function that needs to be overriden for Destroy
	*************************************************************************/
	void CPPSTutorial4::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	Changes the collided variable to true when the MainChar has collided with it
	*************************************************************************/
	void CPPSTutorial4::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "MainChar")
		{
			collided = true;
		}
	}
}