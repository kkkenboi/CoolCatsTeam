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
		mGameManager = GOMANAGER->FindGameObjectWithName("CameraFollow");

		mTransform = GameObj->GetComponent<CPTransform>();
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
			// Turn the directionhelper on
			GameObj->GetComponent<CPRender>()->ToggleActive(true);

			//Setting the position of the aim script to the player
			mTransform->SetPosition(mPlayerTransform->GetPosition());

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

			// Get the direction to event, as well as how far it is away from the screen since it is a relative length now
			// Event Position - Player Position = Distance from player to event
			DirToEvent = mEventTransform->GetPosition() - mTransform->GetPosition();

			Vec2<Vec2<float>> CurrentEdgePosition = mTransform->GetPosition() + 
			
			// To REDO: If using UI layer, the screen space is actually from 0 to 1960 for x and 0 to 540 for y
			// If the length is lesser or greater than the screen size, show the direction helper
			if (DirToEvent.x > 1920.f || DirToEvent.y > 1080.f || DirToEvent.x < 0.f || DirToEvent.y < 0.f)
			{

				// Find where the direction helper should be placed before the border
				Vec2<float> borderPos{};

				// - Setting one/two of the max values
				if (DirToEvent.x > 1920.f) {
					borderPos.x = 1900.f;
					borderPos.y = 1080.f - DirToEvent.y;
				}
				else if (DirToEvent.x < 0.f) {
					borderPos.x = 20.f;
					borderPos.y = 1080.f - DirToEvent.y;
				}

				else if (DirToEvent.y > 1080.f) {
					borderPos.x = DirToEvent.x;
					borderPos.y = 1060.f;
				}
				else if (DirToEvent.y < 0.f) {
					borderPos.x = DirToEvent.x;
					borderPos.y = 20.f;
				}

				// Normalise to just get the general direction
				DirToEvent.Normalise();

				// Get the position before the border of the screen, update the position and rotation
				mTransform->SetPosition(borderPos);
				mTransform->SetRotation(RadToDeg(atan2f(DirToEvent.y, DirToEvent.x)));
			}
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
