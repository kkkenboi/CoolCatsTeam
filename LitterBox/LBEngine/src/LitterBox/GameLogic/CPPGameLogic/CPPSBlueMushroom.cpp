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
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");

		mToMaxTimer = { 0.15f };
		mToMinTimer = { 0.35f };
		mScaleTimer = { 0.35f };

		mScaleTimerRemaining = { 0.35f };

		mScaleOG = { 1.f,1.f };
		mScaleMax = { 1.25f , 1.25f };

		mScaledUp = false;
		mScaledDown = true;
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
	Overriden destroy function because of inheritance
	*************************************************************************/
	void CPPSBlueMushroom::Destroy()
	{

	}
}