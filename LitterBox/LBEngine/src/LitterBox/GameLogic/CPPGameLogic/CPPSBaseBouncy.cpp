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


#include "CPPSBaseBouncy.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Audio/AudioManager.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Every time the bouncy collides with anything its scale changes
	*************************************************************************/
	void CPPSBaseBouncy::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->rigidbody != nullptr)
		{
			if (colData.colliderOther->gameObj->GetName() != "ball" || colData.colliderOther->gameObj->GetName() != "Projectile")
			{
				//Modify this value to change the velocity of the bounce
				colData.colliderOther->rigidbody->mVelocity.x *= bounceForce;
				colData.colliderOther->rigidbody->mVelocity.y *= bounceForce;
			}
			if (colData.colliderOther->gameObj->GetName() == "ball")
			{
				colData.colliderOther->rigidbody->mVelocity.x *= 1.1f;
				colData.colliderOther->rigidbody->mVelocity.y *= 1.1f;
			}

			if (colData.colliderOther->gameObj == mPlayer) {
				//std::cout << "hitting player!" << std::endl;
				//std::cout << "Before : " << mPlayer->GetComponent<CPRigidBody>()->mVelocity.x <<
				//	", " << mPlayer->GetComponent<CPRigidBody>()->mVelocity.y << std::endl;
				Vec2<float> bouncyPos = colData.colliderThis->transform->GetPosition();
				Vec2<float> playerPos = colData.colliderOther->transform->GetPosition();

				Vec2<float> forceToApply = playerPos - bouncyPos;
				
				//std::cout << "After : " << mPlayer->GetComponent<CPRigidBody>()->mVelocity.x <<
				//	", " << mPlayer->GetComponent<CPRigidBody>()->mVelocity.y << std::endl;
			}
			if (GameObj->GetName() == "Bramble")
			{
				int Channel = AUDIOMANAGER->PlaySound("Bramble");
				AUDIOMANAGER->SetChannelVolume(Channel, 0.2f);
			}
			else
			{
				int chnl = AUDIOMANAGER->PlaySound("Shroom Bounce");
				AUDIOMANAGER->SetChannelVolume(chnl, 0.2f);

			}
			int Channel = AUDIOMANAGER->PlaySound("Thud");
			AUDIOMANAGER->SetChannelVolume(Channel, 0.2f);
				

			// Start scaling down in the next update loop
			mScaleTimer = mToMaxTimer;
			mScaleTimerRemaining = mScaleTimer;
			mScaledUp = true;
		}
	}

	/*!***********************************************************************
	\brief
	Gets the object's render component
	*************************************************************************/
	CPRender* CPPSBaseBouncy::GetRender()
	{
		return mRender;
	}

	/*!***********************************************************************
	\brief
	Gets the object's rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSBaseBouncy::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Gets the objcet's collider component
	*************************************************************************/
	CPCollider* CPPSBaseBouncy::GetCollider()
	{
		return mCollider;
	}

	// Helper function
	Vec2<float> VecLerp(const Vec2<float>& a, const Vec2<float>& b, float t)
	{
		return Vec2<float>{a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
	}

}