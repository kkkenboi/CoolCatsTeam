/*!************************************************************************
 \file				CPPSBush.cpp
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

#include "CPPSBush.h"
#include "LitterBox/Audio/AudioManager.h"

namespace LB
{
	void CPPSBush::Start()
	{
		mTransform = GameObj->GetComponent<CPTransform>()->GetChild();
		mRender = mTransform->GetComponent<CPRender>();
		mCollider = GameObj->GetComponent<CPCollider>();
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");

		mToMaxTimer = { 0.15f };
		mToMinTimer = { 0.35f };
		mScaleTimer = { 0.35f };

		mScaleTimerRemaining = { 0.35f };

		mScaleOG = { 1.f,1.f };
		mScaleMax = { 1.25f , 1.25f };

		mScaledUp = false;
		mScaledDown = true;

		// Randomize bush sprite
		mRender->spriteIndex = 45 + (rand() % 3);
		mRender->SetSpriteTexture(mRender->spriteSheetName, mRender->spriteIndex);
	}

	void CPPSBush::Update()
	{
		if (mScaledUp)
		{
			// Calculate the interpolation factor
			float t = 1.0f - (mScaleTimerRemaining / mScaleTimer);

			// Lerp towards the target scale
			Vec2<float> lerpedScale = VecLerp(mTransform->GetLocalScale(), mScaleMax, t);
			mTransform->SetScale(lerpedScale);

			// Update the timer
			mScaleTimerRemaining -= static_cast<float>(TIME->GetDeltaTime());

			if (mScaleTimerRemaining <= 0.f) {
				// Ensure that the scale reaches the target exactly
				mTransform->SetScale(Vec2<float>{1.25f, 1.25f});

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
			mTransform->SetScale(lerpedScale);

			// Update the timer
			mScaleTimerRemaining -= static_cast<float>(TIME->GetDeltaTime());

			if (mScaleTimerRemaining <= 0.f) {
				// Ensure that the scale reaches the target exactly
				mTransform->SetScale(Vec2<float>{1.f, 1.f});

				// Reset the timer and flags
				mScaleTimerRemaining = mScaleTimer;
				mScaledDown = false;
			}
		}
	}

	void CPPSBush::Destroy()
	{

	}

	void CPPSBush::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->rigidbody != nullptr)
		{
			// Start scaling down in the next update loop
			mScaleTimer = mToMaxTimer;
			mScaleTimerRemaining = mScaleTimer;
			mScaledUp = true;

			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->BushSounds, 0.15f);
		}

		if (colData.colliderOther->gameObj->GetName() == "ball")
		{

		}
	}
}