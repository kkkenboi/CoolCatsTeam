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

#include "CPPSSandpit.h"
#include "LitterBox/Animation/ParticleSystem.h"
#include "CPPSPlayer.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSSandpit::Start()
	{
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
	}

	/*!***********************************************************************
	\brief
	Update function where the mushroom's scale is being changed
	*************************************************************************/
	void CPPSSandpit::Update()
	{

	}

	/*!***********************************************************************
	\brief
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSSandpit::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	Every time the mushroom collides with anything it's scale changes
	*************************************************************************/
	void CPPSSandpit::OnCollisionEnter(CollisionData colData)
	{
		UNREFERENCED_PARAMETER(colData);
	}

	/*!***********************************************************************
	\brief
	Happens every time its currently colliding with another object
	*************************************************************************/
	void CPPSSandpit::OnCollisionStay(CollisionData colData)
	{
		if (colData.colliderOther->rigidbody != nullptr)
		{
			if (colData.colliderOther->gameObj->GetName() != "Projectile" && colData.colliderOther->gameObj->GetName() != "Mage")
			{
				if (colData.colliderOther->gameObj->GetName() == "ball") {
					colData.colliderOther->rigidbody->mVelocity.x *= 0.925f;
					colData.colliderOther->rigidbody->mVelocity.y *= 0.925f;
				}
				else if (colData.colliderOther->gameObj->GetName() == "MainChar") 
				{
					colData.colliderOther->rigidbody->mVelocity.x *= 0.9f;
					colData.colliderOther->rigidbody->mVelocity.y *= 0.9f;
					colData.colliderOther->gameObj->GetComponent<CPPSPlayer>()->isOnSand = true;
				}
				else
				{
					colData.colliderOther->rigidbody->mVelocity.x *= 0.95f;
					colData.colliderOther->rigidbody->mVelocity.y *= 0.95f;
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
	Happens every time an object stops colliding with the sandpit
	*************************************************************************/
	void CPPSSandpit::OnCollisionExit(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "MainChar")
		{
			colData.colliderOther->gameObj->GetComponent<CPPSPlayer>()->isOnSand = false;
		}

	}
}