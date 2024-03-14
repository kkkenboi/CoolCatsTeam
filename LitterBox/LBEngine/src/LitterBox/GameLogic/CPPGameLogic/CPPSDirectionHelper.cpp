/*!************************************************************************
 * \file				CPPSDirectionHelper.cpp
 * \author(s)			Kenji Brannon Chong
 * \par DP email(s):  	kenjibrannon.c@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This file contains the source code for the Direction 
 *						Helper which points the player in the general direction
 *						when the game is over.
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSDirectionHelper.h"
#include "CPPSCameraFollow.h"
#include "CPPGameManager.h"
#include <algorithm>

namespace LB
{
	/*!************************************************************************
	 * \brief Start function of the AIM UI
	 * It gets the player GO and its own transform and caches it
	**************************************************************************/
	void CPPSDirectionHelper::Start()
	{
		// Initialising pointers for easy reference
		mPlayerTransform = GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPTransform>();
		mCameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");
		mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");

		mTransform = GameObj->GetComponent<CPTransform>();


		// Set how far direction helper should be from the player
		distance = 400.0f;
	}

	/*!************************************************************************
	 * \brief Uudate function for the Aim UI, basically just points the
	 * image towards the player's cursor
	**************************************************************************/
	void CPPSDirectionHelper::Update()
	{
		// If left 1 enemy, show position of last enemy
		if (mLastEnemy)
		{

			if (GOMANAGER->FindGameObjectWithName("EnemyChaser1"))
			{
				mEventTransform = GOMANAGER->FindGameObjectWithName("EnemyChaser1")->GetComponent<CPTransform>();
			}
			if (GOMANAGER->FindGameObjectWithName("Mage"))
			{
				mEventTransform = GOMANAGER->FindGameObjectWithName("Mage")->GetComponent<CPTransform>();
			}
			if (GOMANAGER->FindGameObjectWithName("Charger_Shield"))
			{
				mEventTransform = GOMANAGER->FindGameObjectWithName("Charger_Shield")->GetComponent<CPTransform>();
			}

			DirToEvent = mEventTransform->GetPosition() - mPlayerTransform->GetPosition();

			Vec2<Vec2<float>> CurrentScreenBorder = { (mPlayerTransform->GetPosition() + Vec2<float>{800.f, 450.f}), (mPlayerTransform->GetPosition() - Vec2<float>{800.f, 450.f}) };

			bool EnemyNotWithinScreen = !(mEventTransform->GetPosition().x < CurrentScreenBorder.x.x && mEventTransform->GetPosition().x > CurrentScreenBorder.y.x && mEventTransform->GetPosition().y < CurrentScreenBorder.x.y && mEventTransform->GetPosition().y > CurrentScreenBorder.y.y);
			if (EnemyNotWithinScreen)
			{
				// Turn the direction helper on
				GameObj->GetComponent<CPRender>()->ToggleActive(true);

				DirToEvent.Normalise();

				// Set position and direction of the direction helper
				mTransform->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * distance);
				mTransform->SetRotation(RadToDeg(atan2f(DirToEvent.y, DirToEvent.x)));

				// Indicate which enemy is left

			}
			else
			{
				// Turn the direction helper off
				GameObj->GetComponent<CPRender>()->ToggleActive(false);
			}

			// Find where the direction helper should be placed 
			//Vec2<float> {};

			//// If top right portion of the screen
			//if (DirToEvent.x > 0.f && DirToEvent.y > 0.f)
			//{
			//	cosTheta = DotProduct(DirToEvent, Vec2<float>(1.f, 0.f));

			//	// If right-top border of the screen
			//	if (acos(cosTheta) <= 0.785f)
			//	{
			//		borderPos.x = mPlayerTransform->GetPosition().x + 960.f;
			//		borderPos.y = mPlayerTransform->GetPosition().y + (960.f / cosTheta);
			//	}
			//	// If top-right border of the screen
			//	else if (acos(cosTheta) > 0.785f)
			//	{
			//		cosTheta = DotProduct(DirToEvent, Vec2<float>(0.f, 1.f));
			//		borderPos.x = mPlayerTransform->GetPosition().x + (540.f / cosTheta);
			//		borderPos.y = mPlayerTransform->GetPosition().y + 540.f;
			//	}

			//}
			//else if (DirToEvent.x < 0.f && DirToEvent.y < 0.f)
			//{

			//}
			//else if (DirToEvent.x < 0.f && DirToEvent.y > 0.f)
			//{

			//}
			//else if (DirToEvent.x > 0.f && DirToEvent.y < 0.f)
			//{

			//}

			//float angle = atan2f(DirToEvent.y, DirToEvent.x);
			//float cosTheta = cos(angle);
			//float sinTheta = sin(angle);

			//// Calculate the distance to the borders based on the angle
			//float distanceToRightBorder = 960.0f / cosTheta;
			//float distanceToLeftBorder = -960.0f / cosTheta;
			//float distanceToTopBorder = 540.0f / sinTheta;
			//float distanceToBottomBorder = -540.0f / sinTheta;

			//// Calculate the final position of the direction helper based on the angle and border distances
			//if (cosTheta >= 0 && sinTheta >= 0) // Top-right
			//{
			//	borderPos.x = mPlayerTransform->GetPosition().x + distanceToRightBorder * cosTheta;
			//	borderPos.y = mPlayerTransform->GetPosition().y + distanceToTopBorder * sinTheta;
			//}
			//else if (cosTheta >= 0 && sinTheta < 0) // Bottom-right
			//{
			//	borderPos.x = mPlayerTransform->GetPosition().x + distanceToRightBorder * cosTheta;
			//	borderPos.y = mPlayerTransform->GetPosition().y + distanceToBottomBorder * sinTheta;
			//}
			//else if (cosTheta < 0 && sinTheta >= 0) // Top-left
			//{
			//	borderPos.x = mPlayerTransform->GetPosition().x + distanceToLeftBorder * cosTheta;
			//	borderPos.y = mPlayerTransform->GetPosition().y + distanceToTopBorder * sinTheta;
			//}
			//else if (cosTheta < 0 && sinTheta < 0) // Bottom-left
			//{
			//	borderPos.x = mPlayerTransform->GetPosition().x + distanceToLeftBorder * cosTheta;
			//	borderPos.y = mPlayerTransform->GetPosition().y + distanceToBottomBorder * sinTheta;
			//}
			//// Normalise to just get the general direction
			//DirToEvent.Normalise();

			//// Get the position before the border of the screen, update the position and rotation
			//mTransform->SetPosition(borderPos);
			//mTransform->SetRotation(RadToDeg(atan2f(DirToEvent.y, DirToEvent.x)));
		}
		else if (mUpgradeSpawned)
		{
			// Turn the directionhelper on
			GameObj->GetComponent<CPRender>()->ToggleActive(true);
		}
		else
		{
			// Turn the directionhelper off
			GameObj->GetComponent<CPRender>()->ToggleActive(false);
		}

	}

	/*!************************************************************************
	 * \brief Handles the destruction of the Aim UI. Should be empty for now
	 *
	**************************************************************************/
	void CPPSDirectionHelper::Destroy()
	{
		//Should be empty
	}
}
