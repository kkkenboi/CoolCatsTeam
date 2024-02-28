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
			//If upgrade has been hit, it tells the UpgradeManager by calling set ball upgrade		
			//CPPSUpgradeManager::Instance()->SetBallUpgrade(assignedUpgradeType);
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
				break;
			case MOVESPEED:
				GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->m_PlayerWalkSpeed *= 1.5f;
				GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->m_PlayerArbitraryFriction *= 0.8f;
				break;
			case BIGBALL:
				// std::cout << "Bigger Balls!\n";

			default:
				//do nothing
				break;
			}
		/*	BallGameObj = GOMANAGER->FindGameObjectWithName("ball");
			BallGameObj->GetComponent<CPPSPlayerGolfBall>()->SetBallUpgrade(1);*/
			canDestroy = true;
			GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->HideUpgrades(assignedUpgradeType);
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