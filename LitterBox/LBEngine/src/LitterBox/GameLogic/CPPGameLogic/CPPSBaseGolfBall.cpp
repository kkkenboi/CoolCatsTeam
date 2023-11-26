/*!************************************************************************
 \file				CPPSBaseGolfBall.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the Projectile Ball behaviour, Name of script will be changed in the
 future.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSBaseGolfBall.h"

#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Enter the state of chasing where it will initialise the values
	*************************************************************************/
	void CPPSBaseGolfBall::Start()
	{
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();

		std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") 
			{
				mPlayer = GO;
				break;
			}
		}

		// Set direction (rotation)
		CPTransform* trans = GameObj->GetComponent<CPTransform>();
		Vec2<float> currPos = trans->GetPosition();
		Vec2<float> shootDir = currPos - mPlayer->GetComponent<CPTransform>()->GetPosition();

		trans->SetRotation(RadToDeg(DotProduct(shootDir.Normalise(), Vec2<float>{1.0f, 1.0f })));
		trans->SetRotation(trans->GetRotation() + 90.0f);

		mRigidBody->mFriction = 1.0f;
		mSpeedMagnitude = 1000.0f;
		mVelocity = 1000.0f; //with direction
		mSize = 1.0f;
	}

	void CPPSBaseGolfBall::Update() { }

	void CPPSBaseGolfBall::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->m_gameobj->GetName() == "MainChar" ||
			colData.colliderOther->m_gameobj->GetName() == "NorthWall" ||
			colData.colliderOther->m_gameobj->GetName() == "SouthWall" ||
			colData.colliderOther->m_gameobj->GetName() == "WestWall" ||
			colData.colliderOther->m_gameobj->GetName() == "EastWall")
		{
			
			int Channel = AUDIOMANAGER->PlaySound("Smoke Poof by sushiman2000 Id - 643876");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.5f);

			GOMANAGER->RemoveGameObject(this->GameObj);
		}
		
	}

	void CPPSBaseGolfBall::Destroy() { }

	//Getter functions
	/*!***********************************************************************
	\brief
	Getter for the render component
	*************************************************************************/
	CPRender* CPPSBaseGolfBall::GetRender()
	{
		return mRender;
	}
	/*!***********************************************************************
	\brief
	Getter for the rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSBaseGolfBall::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Getter for the collider component
	*************************************************************************/
	CPCollider* CPPSBaseGolfBall::GetCollider()
	{
		return mCollider;
	}

	/*!***********************************************************************
	\brief
	Getter for the player object
	*************************************************************************/
	GameObject* CPPSBaseGolfBall::GetHero()
	{
		return mPlayer;
	}
}

