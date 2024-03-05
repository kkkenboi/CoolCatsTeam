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
			colData.colliderOther->rigidbody->mVelocity.x *= 1.75f;
			colData.colliderOther->rigidbody->mVelocity.y *= 1.75f;

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

			// Start scaling down in the next update loop
			mScaleTimer = mToMaxTimer;
			mScaleTimerRemaining = mScaleTimer;
			mScaledUp = true;
		}
	}

	/*!***********************************************************************
	\brief
	To the sprite scale when interacting with other game objects
	*************************************************************************/
	void CPPSBaseBouncy::OnInteraction(int spriteIndexStart, int spriteIndexEnd)
	{
		if (mScaledUp || mScaledDown)
		{
			mRender->SetSpriteTexture(mRender->spriteSheetName, spriteIndexEnd);
		}
		else
		{
			mRender->SetSpriteTexture(mRender->spriteSheetName, spriteIndexStart);
		}
		if (mScaledUp)
		{
			// Calculate the interpolation factor
			float t = 1.0f - (mScaleTimerRemaining / mScaleTimer);

			// Lerp towards the target scale
			Vec2<float> lerpedScale = VecLerp(mTransform->GetLocalScale(), mScaleMax, t);
			GameObj->GetComponent<CPTransform>()->SetScale(lerpedScale);

			// Update the timer
			mScaleTimerRemaining -= static_cast<float>(TIME->GetDeltaTime());

			if (mScaleTimerRemaining <= 0.f) {
				// Ensure that the scale reaches the target exactly
				GameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.25f, 1.25f});

				// Reset the timer and flags
				mScaleTimer = mToMinTimer;
				mScaleTimerRemaining = mScaleTimer;
				mScaledUp = false;
				mScaledDown = true; // Start scaling down
			}
		}
		else if (mScaledDown)
		{
			// Calculate the interpolation factor
			float t = 1.0f - (mScaleTimerRemaining / mScaleTimer);

			// Lerp towards the original scale
			Vec2<float> lerpedScale = VecLerp(mTransform->GetLocalScale(), mScaleOG, t);
			GameObj->GetComponent<CPTransform>()->SetScale(lerpedScale);

			// Update the timer
			mScaleTimerRemaining -= static_cast<float>(TIME->GetDeltaTime());

			if (mScaleTimerRemaining <= 0.f) {
				// Ensure that the scale reaches the target exactly
				GameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{1.f, 1.f});

				// Reset the timer and flags
				mScaleTimerRemaining = mScaleTimer;
				mScaledDown = false;
			}
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