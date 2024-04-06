/*!************************************************************************
 * \file				CPPSUpgrade.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This source file contains the function definitions
 * 						for the upgrade prefab
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSUpgrade.h"
#include "CPPSPlayerGolfBall.h"
#include "CPPSUpgradeManager.h"
#include "CPPSPlayerHUD.h"
#include "CPPSPlayer.h"
#include "CPPGameManager.h"
#include "CPPVFXManager.h"
namespace LB
{
	/*!************************************************************************
	 * \brief Start function for the upgrade (Currently does nothing)
	 * 
	**************************************************************************/
	void CPPSUpgrade::Start()
	{
		//Grabbing the ball reference
		//BallGameObj = GOMANAGER->FindGameObjectWithName("ball");
		//assignedUpgradeType = (std::rand() % 3) + 1;
		
	}

	/*!************************************************************************
	 * \brief Update function for the upgrade, currently checks if 
	 * the object should be destroyed or not
	**************************************************************************/
	void CPPSUpgrade::Update()
	{
		if(canDestroy) GOMANAGER->RemoveGameObject(this->GameObj);
	}
	/*!************************************************************************
	 * \brief Destroy function for the upgrade, currently does nothing
	 * 
	**************************************************************************/
	void CPPSUpgrade::Destroy()
	{
	}

	/*!************************************************************************
	 * \brief Function that handles the collision with the upgrade
	 * 
	 * \param colData Collision data of the collision with the upgrade
	**************************************************************************/
	void CPPSUpgrade::OnCollisionEnter(CollisionData colData)
	{
		//make collision layer that only contains ball and upgrade interaction in the future
		//If the upgrade collides with a ball
		if (colData.colliderOther->m_gameobj->GetName() == "ball")
		{
			auto gameManager = GOMANAGER->FindGameObjectWithName("GameManager");
			if (gameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentUpgrades < gameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxUpgrades)
			{
				GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->m_mouseHoverWorld = false;
				//If upgrade has been hit, it tells the UpgradeManager by calling set ball upgrade		
				//CPPSUpgradeManager::Instance()->SetBallUpgrade(assignedUpgradeType);

				//This prevents the player from getting the same upgrade twice
				if (GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->HasUpgrade(assignedUpgradeType))
				{
					return;
				}

				GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->SetBallUpgrade(assignedUpgradeType);
				switch (assignedUpgradeType)
				{
				case MOREBALL:
					// Should move functions from PlayerHUD to the GameManager in the future
					GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(3);
					break;
				case MOREHEALTH:
					// Should move functions from PlayerHUD to the GameManager in the future
					GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(3);
					GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPPSPlayer>()->anim->PlayAndReset("Felix_Walk");
					break;
				case MOVESPEED:
					GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed *= 1.5f;
					GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->m_PlayerArbitraryFriction *= 0.8f;
					break;
				case BIGBALL:
					// std::cout << "Bigger Balls!\n";
					break;
				case GOLFSWING:
					GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnPoofAnim(GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPTransform>()->GetPosition(),2.f);
					GOMANAGER->FindGameObjectWithName("MainChar")->GetComponent<CPPSPlayer>()->m_shootRadius *= 2;
					GOMANAGER->FindGameObjectWithName("Club")->GetComponent<CPRender>()->SetSpriteTexture(GOMANAGER->FindGameObjectWithName("Club")->GetComponent<CPRender>()->spriteSheetName, 4);
					GOMANAGER->FindGameObjectWithName("Club")->GetComponent<CPRender>()->w = 125;
					GOMANAGER->FindGameObjectWithName("Club")->GetComponent<CPRender>()->h = 125;

					break;
				default:
					//do nothing
					break;
				}
			
			}

			canDestroy = true;
			GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->HideUpgrades(assignedUpgradeType);
			if (GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->GetCurrentWave() == 0) return;
			switch (GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->upgradeCount)
			{
			case 1:
				
				GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost1->SetSpriteTexture("MultiSheet2", 3 + assignedUpgradeType);
				//GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost1->gameObj->SetActive(true);
				break;
			case 2:
				GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost2->SetSpriteTexture("MultiSheet2", 3 + assignedUpgradeType);
				//GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost2->gameObj->SetActive(true);
				break;
			case 3:
				GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost3->SetSpriteTexture("MultiSheet2", 3 + assignedUpgradeType);
				//GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost3->gameObj->SetActive(true);

				break;
			case 4:
				GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost4->SetSpriteTexture("MultiSheet2", 3 + assignedUpgradeType);
				//GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ItemLost4->gameObj->SetActive(true);

				break;
			default:
				DebuggerLogWarningFormat("Invalid upgrade count of %d!", GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->upgradeCount);
				break;
			}
			/*this->GameObj->SetActive(false);*/
			//std::cout << this->GameObj->GetName() << '\n';
			//GameObj->GetComponent<CPTransform>()->SetPosition({ 10000.f,10000.f });
			//GOMANAGER->RemoveGameObject(this->GameObj);

		}
	}

	/*!************************************************************************
	 * \brief Function to assign the upgrade, it gets called by the upgrade manager
	 * and it assigns the current type of upgrade to the ball
	 * \param upgradeType Type of upgrade to assign
	**************************************************************************/
	void CPPSUpgrade::AssignUpgradeID(int upgradeType)
	{
	//Gets called by the upgrade manager, to assign the current type of upgrade to the ball
		//Called by the upgrade 
		assignedUpgradeType = upgradeType;
	}
}