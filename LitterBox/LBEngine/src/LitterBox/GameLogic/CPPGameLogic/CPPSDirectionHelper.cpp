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
#include "CPPSUpgradeManager.h"
#include <algorithm>

namespace LB
{
	/*!************************************************************************
	* \brief Start function for the Direction Helper
	*		 It gets the the needed pointers to different components and Game
	*		 Objects for easy access
	**************************************************************************/
	void CPPSDirectionHelper::Start()
	{
		// Initialising pointers for easy reference
		mPlayerTransform = GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPTransform>();
		mCameraFollow = GOMANAGER->FindGameObjectWithName("CameraFollow");
		mGameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		mIconObject = GOMANAGER->FindGameObjectWithName("IconObject");
		mPortal = GOMANAGER->FindGameObjectWithName("Portal");

		mTransform = GameObj->GetComponent<CPTransform>();


		// Set how far direction helper should be from the player
		distance = 400.0f;
		iconDistance = 340.0f;
	}

	/*!************************************************************************
	* \brief Update function for Direction Helper to update when there is only
	*		 one enemy left or if the upgrade is not taken yet
	**************************************************************************/
	void CPPSDirectionHelper::Update()
	{
		bool EventNotWithinScreen{};
		// If left 1 enemy, show position of last enemy
		if (mGameManager->GetComponent<CPPSGameManager>()->GetSpawnedEnemyList().size() == 1)
		{
			mEventTransform = mGameManager->GetComponent<CPPSGameManager>()->GetSpawnedEnemyList()[0]->GetComponent<CPTransform>();

			DirToEvent = mEventTransform->GetPosition() - mPlayerTransform->GetPosition();

			Vec2<Vec2<float>> CurrentScreenBorder = { (mPlayerTransform->GetPosition() + Vec2<float>{900.f, 600.f}), (mPlayerTransform->GetPosition() - Vec2<float>{900.f, 600.f}) };

			EventNotWithinScreen = !(mEventTransform->GetPosition().x < CurrentScreenBorder.x.x && mEventTransform->GetPosition().x > CurrentScreenBorder.y.x && mEventTransform->GetPosition().y < CurrentScreenBorder.x.y && mEventTransform->GetPosition().y > CurrentScreenBorder.y.y);
			if (EventNotWithinScreen)
			{
				// Set position and direction of the direction helper
				mTransform->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * distance);
				mTransform->SetRotation(RadToDeg(atan2f(DirToEvent.y, DirToEvent.x)));

				// Set position of iconObject and texture
				mIconObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * iconDistance);
				auto& EnemyName = mGameManager->GetComponent<CPPSGameManager>()->GetSpawnedEnemyList()[0]->GetName();
				//DebuggerLogFormat("Last Enemy : %s", EnemyName);
				if (EnemyName == "EnemyChaser1")
				{
					//DebuggerLog("Entering");
					mIconObject->GetComponent<CPRender>()->SetSpriteTexture("FelixSheet", 54);
				}
				else if (EnemyName == "Mage")
				{
					mIconObject->GetComponent<CPRender>()->SetSpriteTexture("FelixSheet", 55);
				}
				else if (EnemyName == "Charger_Shield")
				{
					mIconObject->GetComponent<CPRender>()->SetSpriteTexture("FelixSheet", 56);
				}

				// Render both direction helper and iconObject
				GameObj->GetComponent<CPRender>()->ToggleActive(true);
				mIconObject->GetComponent<CPRender>()->ToggleActive(true);

			}
			else
			{
				// Turn the direction helper off
				GameObj->GetComponent<CPRender>()->ToggleActive(false);
				mIconObject->GetComponent<CPRender>()->ToggleActive(false);
			}

		}
		else if (GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->GetSpawned() && !mUpgradeTaken)
		{
			
			mEventTransform = GOMANAGER->FindGameObjectWithName("middleUpgrade")->GetComponent<CPTransform>();
			//DebuggerLogFormat("Last Enemy : %s", mGameManager->GetComponent<CPPSGameManager>()->GetSpawnedEnemyList()[0]->GetName().c_str());

			DirToEvent = mEventTransform->GetPosition() - mPlayerTransform->GetPosition();

			Vec2<Vec2<float>> CurrentScreenBorder = { (mPlayerTransform->GetPosition() + Vec2<float>{900.f, 600.f}), (mPlayerTransform->GetPosition() - Vec2<float>{900.f, 600.f}) };

			EventNotWithinScreen = !(mEventTransform->GetPosition().x < CurrentScreenBorder.x.x&& mEventTransform->GetPosition().x > CurrentScreenBorder.y.x && mEventTransform->GetPosition().y < CurrentScreenBorder.x.y&& mEventTransform->GetPosition().y > CurrentScreenBorder.y.y);
			if (EventNotWithinScreen)
			{
				// Set position and direction of the direction helper
				mTransform->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * distance);
				mTransform->SetRotation(RadToDeg(atan2f(DirToEvent.y, DirToEvent.x)));

				// Set position of iconObject and texture
				mIconObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * iconDistance);
				mIconObject->GetComponent<CPRender>()->SetSpriteTexture("FelixSheet", 57);

				// Render both direction helper and iconObject
				GameObj->GetComponent<CPRender>()->ToggleActive(true);
				mIconObject->GetComponent<CPRender>()->ToggleActive(true);
			}
			else
			{
				// Stop rendering both direction helper and iconObject 
				GameObj->GetComponent<CPRender>()->ToggleActive(false);
				mIconObject->GetComponent<CPRender>()->ToggleActive(false);
			}
		}
		else if (mPortal->IsActive())
		{
			mEventTransform = mPortal->GetComponent<CPTransform>();

			DirToEvent = mEventTransform->GetPosition() - mPlayerTransform->GetPosition();

			Vec2<Vec2<float>> CurrentScreenBorder = { (mPlayerTransform->GetPosition() + Vec2<float>{900.f, 600.f}), (mPlayerTransform->GetPosition() - Vec2<float>{900.f, 600.f}) };

			EventNotWithinScreen = !(mEventTransform->GetPosition().x < CurrentScreenBorder.x.x && mEventTransform->GetPosition().x > CurrentScreenBorder.y.x && mEventTransform->GetPosition().y < CurrentScreenBorder.x.y && mEventTransform->GetPosition().y > CurrentScreenBorder.y.y);
			if (EventNotWithinScreen)
			{
				// Set position and direction of the direction helper
				mTransform->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * distance);
				mTransform->SetRotation(RadToDeg(atan2f(DirToEvent.y, DirToEvent.x)));

				// Set position of iconObject and texture
				mIconObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f} + DirToEvent.Normalise() * iconDistance);
				mIconObject->GetComponent<CPRender>()->SetSpriteTexture("FelixSheet", 58);

				// Render both direction helper and iconObject
				GameObj->GetComponent<CPRender>()->ToggleActive(true);
				mIconObject->GetComponent<CPRender>()->ToggleActive(true);
			}
			else
			{
				// Stop rendering both direction helper and iconObject 
				GameObj->GetComponent<CPRender>()->ToggleActive(false);
				mIconObject->GetComponent<CPRender>()->ToggleActive(false);
			}
		}
		else
		{
			// Turn the direction helper off
			GameObj->GetComponent<CPRender>()->ToggleActive(false);
			mIconObject->GetComponent<CPRender>()->ToggleActive(false);
		}

	}

	/*!************************************************************************
	* \brief Destroy function for the Direction Helper.
	*
	**************************************************************************/
	void CPPSDirectionHelper::Destroy()
	{
		//Should be empty
	}
}
