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

#include "math.h"	//Only using it for atan2

namespace LB
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSBaseGolfBall::Start()
	{
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();
		//Getting the player GO reference
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		/*std::vector<GameObject*> const& GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") 
			{
				mPlayer = GO;
				break;
			}
		}*/

		// Set direction (rotation)
		//CPTransform* trans = GameObj->GetComponent<CPTransform>();
		//Vec2<float> currPos = trans->GetPosition();
		//Vec2<float> shootDir = mRigidBody->mVelocity;
		//shootDir.Normalise();
			//mPlayer->GetComponent<CPTransform>()->GetPosition() - currPos;
		//trans->SetRotation(RadToDeg(atan2f(shootDir.y, shootDir.x)));
		/*trans->SetRotation(RadToDeg(DotProduct(shootDir.Normalise(), Vec2<float>{1.0f, 1.0f })));
		trans->SetRotation(trans->GetRotation() + 90.0f);*/

		GetComponent<CPAnimator>()->PlayRepeat("MageProjectileMove");

		mRigidBody->mFriction = 1.0f;
		mSpeedMagnitude = 1000.0f;
		mVelocity = 1000.0f; //with direction
		mSize = 1.0f;
	}
	
	/*!***********************************************************************
	\brief
	Update is where the behaviour of the projectile will be updated every frame
	*************************************************************************/
	void CPPSBaseGolfBall::Update() 
	{
		//This sets the rotation for the sprite to always face the direction it's travelling
		CPTransform* trans = GameObj->GetComponent<CPTransform>();
		Vec2<float> shootDir = mRigidBody->mVelocity;
		trans->SetRotation(RadToDeg(atan2f(shootDir.y, shootDir.x)));

	}

	/*!***********************************************************************
	\brief
	On collision to check what is this Projectile is colliding with
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Destroy
	*************************************************************************/
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

