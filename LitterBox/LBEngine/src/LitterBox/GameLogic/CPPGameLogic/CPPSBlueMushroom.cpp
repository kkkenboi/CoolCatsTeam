/*!************************************************************************
 \file				CPPSBlueMushroom.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
  This file contains the behviour of the Blue Mushroom obstacle.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/


#include "CPPSBlueMushroom.h"
#include "LitterBox/Engine/Time.h"


namespace LB 
{
	/*!***********************************************************************
	\brief
	Start function where variables will be initialised
	*************************************************************************/
	void CPPSBlueMushroom::Start()
	{
		mTransform = GameObj->GetComponent<CPTransform>();
		mRender = GameObj->GetComponent<CPRender>();
		mRigidBody = GameObj->GetComponent<CPRigidBody>();
		mCollider = GameObj->GetComponent<CPCollider>();
	}

	/*!***********************************************************************
	\brief
	Update function where the mushroom's scale is being changed
	*************************************************************************/
	void CPPSBlueMushroom::Update()
	{
		if (mScaledUp || mScaledDown) 
		{
			mRender->SetSpriteTexture(mRender->spriteSheetName, 1);
		}
		else 
		{
			mRender->SetSpriteTexture(mRender->spriteSheetName, 0);
		}
		if (mScaledUp)
		{
			// Calculate the interpolation factor
			float t = 1.0f - (mScaleTimerRemaining / mScaleTimer);

			// Lerp towards the target scale
			Vec2<float> lerpedScale = Lerp(mTransform->GetLocalScale(), Vec2<float>{1.25f, 1.25f}, t);
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
			Vec2<float> lerpedScale = Lerp(mTransform->GetLocalScale(), Vec2<float>{1.f, 1.f}, t);
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
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSBlueMushroom::Destroy()
	{

	}

	/*!***********************************************************************
	\brief
	Every time the mushroom collides with anything it's scale changes
	*************************************************************************/
	void CPPSBlueMushroom::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->rigidbody != nullptr)
		{
			colData.colliderOther->rigidbody->mVelocity.x *= 1.75f;
			colData.colliderOther->rigidbody->mVelocity.y *= 1.75f;

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
	CPRender* CPPSBlueMushroom::GetRender()
	{
		return mRender;
	}

	/*!***********************************************************************
	\brief
	Gets the object's rigidbody component
	*************************************************************************/
	CPRigidBody* CPPSBlueMushroom::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Gets the objcet's collider component
	*************************************************************************/
	CPCollider* CPPSBlueMushroom::GetCollider()
	{
		return mCollider;
	}

	// Helper function
	Vec2<float> Lerp(const Vec2<float>& a, const Vec2<float>& b, float t)
	{
		return Vec2<float>{a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
	}

}