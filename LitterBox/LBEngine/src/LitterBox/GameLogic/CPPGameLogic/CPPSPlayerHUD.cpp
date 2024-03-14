/*!************************************************************************
 \file				CPPSPlayerHUD.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
 This file contains the CPPSPlayerHUD class and all its functionalities.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSPlayerHUD.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPSPlayer.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "CPPGameManager.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "CPPSCameraFollow.h"


namespace LB {

	/*!***********************************************************************
	\brief
	Start function to initialise the heart and ball display variables based on
	the current player's variables.
	*************************************************************************/
	void CPPSPlayerHUD::Start()
	{
		// Initialise pointers to specific game objects
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_UpgradeManager = GOMANAGER->FindGameObjectWithName("Upgrade Manager");
		m_Mouse = GOMANAGER->FindGameObjectWithName("MouseCursor");

		// Create horizontal and vertical upgrade popups
		// - The game object now only contains CPRender
		// - Only difference is that the sprite image is set with different scales
		for (int i{1}; i < UpgradeType::MAXCOUNT; ++i)
		{
			GameObject* verticalPopUp = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("VerticalPopUp", *verticalPopUp);
			verticalPopUp->GetComponent<CPRender>()->ToggleActive(false);

			GameObject* horizontalPopUp = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("HorizontalPopUp", *horizontalPopUp);
			horizontalPopUp->GetComponent<CPRender>()->ToggleActive(false);

			// Since UpgradeType's based off enum, name them in that order
			switch (i)
			{
				// Set their names and text for each upgrade
				case SPLIT:
				{
					verticalPopUp->SetName("DuplimirrorVPopUp"), horizontalPopUp->SetName("DuplimirrorHPopUp");
					break;
				}
				case BOMB:
				{
					verticalPopUp->SetName("GolfBombVPopUp"), horizontalPopUp->SetName("GolfBombHPopUp");
					break;
				}
				case BIGBALL:
				{
					verticalPopUp->SetName("GrowthPowderVPopUp"), horizontalPopUp->SetName("GrowthPowderHPopUp");
					break;
				}
				case MOVESPEED:
				{
					verticalPopUp->SetName("BootsOfAgilityVPopUp"), horizontalPopUp->SetName("BootsOfAgilityHPopUp");
					break;
				}
				// This is the slippery upgrade
				case POTION:
				{
					verticalPopUp->SetName("PotionVPopUp"), horizontalPopUp->SetName("PotionHPopUp");
					break;
				}
				case MOREHEALTH:
				{
					verticalPopUp->SetName("HeartLocketVPopUp"), horizontalPopUp->SetName("HeartLocketHPopUp");
					break;
				}
				case MOREBALL:
				{
					verticalPopUp->SetName("BagOfGolfingVPopUp"), horizontalPopUp->SetName("BagOfGolfingHPopUp");
					break;
				}
				default:
					continue;
			}

			verticalPopUp->GetComponent<CPRender>()->SetSpriteTexture(verticalPopUp->GetComponent<CPRender>()->spriteSheetName, i - 1); // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
			horizontalPopUp->GetComponent<CPRender>()->SetSpriteTexture(horizontalPopUp->GetComponent<CPRender>()->spriteSheetName, i + 7); // TO BE EDITED WHEN SPRITESHEET IS RECEIVED

			// Save to all popups container 
			m_totalUpgradePopUps.push_back(std::pair(verticalPopUp, horizontalPopUp));
			//DebuggerLogFormat("Pushed back: %s, %s", verticalPopUp->GetName().c_str(), horizontalPopUp->GetName().c_str());
		}

		//DebuggerLogFormat("Number of m_totalUpgradePopUps: %d", m_totalUpgradePopUps.size());

		// Create game objects to display the health and balls
		// - Health, hardcoding max value cause will be hidden behind
		for (int i{}; i < 9; ++i)
		{
			GameObject* healthObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);

			healthObject->SetName("Heart " + std::to_string(i + 1));

			Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
			healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_heartDisplayOffset.x * i, startPos.y));

			// Set the texture for lost health
			if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				healthObject->GetComponent<CPRender>()->SetSpriteTexture(healthObject->GetComponent<CPRender>()->spriteSheetName, 49);
			}

			// Those extra health we will hide them first, only when upgrades added to make more, turn them on
			if (i >= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth)
			{
				healthObject->GetComponent<CPRender>()->ToggleActive(false);
			}

			m_totalHeartDisplay.push_back(healthObject);
		}

		// - Balls, hardcoding max value cause will be hidden behind
		for (int i{}; i < 9; ++i)
		{
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("BallHUD", *ballObject);

			ballObject->SetName("Ball " + std::to_string(i + 1));

			Vec2 startPos = ballObject->GetComponent<CPTransform>()->GetPosition();
			ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_ballDisplayOffset.x * i, startPos.y));

			// Those extra balls we will hide them first, only when upgrades added to make more, turn them on
			if (i >= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls)
			{
				ballObject->GetComponent<CPRender>()->ToggleActive(false);
			}

			m_totalBallsDisplay.push_back(ballObject);
		}

		// Check if big balls upgrade is obtained
		if (m_UpgradeManager->GetComponent<CPPSUpgradeManager>()->currentBallUpgrades & (1 << (UpgradeType::BIGBALL - 1)))
		{
			activeBallTexture += 2;
			inactiveBallTexture += 2;

			for (int i{}; i < 9; ++i)
			{
				m_totalBallsDisplay[i]->GetComponent<CPTransform>()->SetScale(Vec2<float>(0.9f, 0.9f));
			}
		}

		// - Upgrades
		for (int i{}; i < m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxUpgrades; ++i)
		{
			GameObject* upgradeObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("UpgradeHUD", *upgradeObject);

			upgradeObject->SetName("Upgrade " + std::to_string(i + 1));

			upgradeObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradeObject->GetComponent<CPTransform>()->GetPosition().x + m_upgradeDisplayOffset.x * m_totalUpgradeDisplay.size(),
																				upgradeObject->GetComponent<CPTransform>()->GetPosition().y));

			upgradeObject->GetComponent<CPRender>()->ToggleActive(false);

			m_totalUpgradeDisplay.push_back(upgradeObject);
		}

		// - Backboards
		GameObject* heartBackboard = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("HeartBackboard", *heartBackboard);
		GameObject* ballBackboard = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("BallBackboard", *ballBackboard);
		GameObject* upgradeBackboard = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("UpgradeBackboard", *upgradeBackboard);


		// New Upgrade Event: To know when new upgrades are obtained
		m_UpgradeManager->GetComponent<CPPSUpgradeManager>()->onNewUpgrade.Subscribe(LB::AddNewUpgrade);
	}

	/*!***********************************************************************
	\brief
	Update will reference the player and update the HUD if there are any changes
	such as the health and the balls.
	*************************************************************************/
	void CPPSPlayerHUD::Update()
	{
		// Health Update
		for (size_t i{ 1 }; i <= static_cast<size_t>(m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth); ++i)
		{
			// Set the texture for lost health
			if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->SetSpriteTexture(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->spriteSheetName, 49);
			}
			else if ( i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->SetSpriteTexture(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->spriteSheetName, 48);
			}
		}

		// Ball Update
		for (size_t i{}; i < static_cast<size_t>(m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls); ++i)
		{
			if (i >= static_cast<size_t>(m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls - m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls))
			{
				m_totalBallsDisplay[i]->GetComponent<CPRender>()->SetSpriteTexture(m_totalHeartDisplay[i]->GetComponent<CPRender>()->spriteSheetName, inactiveBallTexture);
			}
			else
			{
				m_totalBallsDisplay[i]->GetComponent<CPRender>()->SetSpriteTexture(m_totalHeartDisplay[i]->GetComponent<CPRender>()->spriteSheetName, activeBallTexture);
			}
		}
		
		// If there is a collision, based on the index given, set the position of the popup and toggleActive
		for (size_t i = 0; i < m_totalUpgradePopUps.size(); ++i)
		{
			if (i + 1 == static_cast<size_t>(m_currentVPopUpIndex))
			{
				//DebuggerLogFormat("m_MouseHoverWorld: %d", m_mouseHoverWorld);
				m_totalUpgradePopUps[i].first->GetComponent<CPRender>()->ToggleActive(m_mouseHoverWorld);
				if (m_playOnceWorld)
				{
					m_totalUpgradePopUps[i].first->GetComponent<CPAnimator>()->PlayAndReset("VFX_Expand");
					m_playOnceWorld = false;
				}
				//DebuggerLogFormat("Number of m_totalUpgradePopUps: %d", m_totalUpgradePopUps.size());
			}
			else
			{
				m_totalUpgradePopUps[i].first->GetComponent<CPRender>()->ToggleActive(false);
			}

			if (i + 1 == static_cast<size_t>(m_currentHPopUpIndex))
			{
				m_totalUpgradePopUps[i].second->GetComponent<CPRender>()->ToggleActive(m_mouseHoverUI);
				if (m_playOnceUI)
				{
					m_totalUpgradePopUps[i].second->GetComponent<CPAnimator>()->PlayAndReset("VFX_Expand");
					m_playOnceUI = false;
				}
			}
			else
			{
				m_totalUpgradePopUps[i].second->GetComponent<CPRender>()->ToggleActive(false);
			}

		}
	}

	/*!***********************************************************************
	\brief
	Inherited Destroy currently doesn't need to do anything.
	*************************************************************************/
	void CPPSPlayerHUD::Destroy()
	{

	}

	/*!***********************************************************************
	 \brief
	 Sets a bool to decrease the HUD's health to true
	*************************************************************************/
	void CPPSPlayerHUD::DecreaseHealth()
	{
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth)
		{
			if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				--m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 Sets a bool to decrease the HUD's ball to true
	*************************************************************************/
	void CPPSPlayerHUD::DecreaseBalls()
	{
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls)
		{
			--m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls;
		}
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for current health in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseHealth()
	{
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth >= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth)
		{
			++m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth;
		}
	}


	/*!***********************************************************************
	 \brief
	 Increases the member variable for current balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseBalls()
	{
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls)
		{
			++m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls;
		}
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for max health in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseMaxHealth(int amount)
	{
		// Prevents player health to be more than 6
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth == 6)
		{
			return;
		}

		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth += amount;
		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth += amount;

		// Sets toggleActive back to true for previous inactive hearts
		for (size_t i{}; i < m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth; ++i)
		{
			m_totalHeartDisplay[i]->GetComponent<CPRender>()->ToggleActive(true);
		}
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for max balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseMaxBalls(int amount)
	{

		// Prevent balls from going more than 6
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls == 6)
		{
			return;
		}

		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls += amount;

		// Sets toggleActive back to true for previous inactive balls
		for (size_t i{}; i < m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls; ++i)
		{
			m_totalBallsDisplay[i]->GetComponent<CPRender>()->ToggleActive(true);
		}
	}

	/*!***********************************************************************
	 \brief
	 To add a new upgrade the player has for the upgrade display in PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::AddNewUpgrade(UpgradeType upgrade)
	{
		// - Check if the upgrade changes any UI backborder visuals
		if (upgrade == UpgradeType::MOREHEALTH)
		{
			GOMANAGER->FindGameObjectWithName("HeartBackboard")->GetComponent<CPRender>()->SetSpriteTexture("BackboardSheet", 1);
		}
		else if (upgrade == UpgradeType::MOREBALL)
		{
			GOMANAGER->FindGameObjectWithName("BallBackboard")->GetComponent<CPRender>()->SetSpriteTexture("BackboardSheet", 3);
		}

		// Check if the upgrade changes the ball texture
		else if (upgrade == UpgradeType::BIGBALL)
		{
			activeBallTexture += 2;
			inactiveBallTexture += 2;

			for (int i{}; i < 9; ++i)
			{
				m_totalBallsDisplay[i]->GetComponent<CPTransform>()->SetScale(Vec2<float>(0.9f, 0.9f));
			}
		}

		// - Set image and spritesheet + index, toggle active to true
		auto upgradePtr = m_totalUpgradeDisplay[m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentUpgrades];
		upgradePtr->GetComponent<CPRender>()->spriteIndex = static_cast<int>(upgrade + 31); // Minus 1 since the enum starts from 1
		upgradePtr->GetComponent<CPRender>()->SetSpriteTexture("MultiSheet", upgradePtr->GetComponent<CPRender>()->spriteIndex); // Minus 1 since the enum starts from 1
		upgradePtr->GetComponent<CPRender>()->ToggleActive(true);

		// - This makes the vertical popup to toggle off
		m_mouseHoverWorld = false;

		// - Set position on screen for the horizontal popup
		auto popUpHPtr = m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)].second;
		popUpHPtr->GetComponent<CPTransform>()->SetPosition(Vec2<float>(popUpHPtr->GetComponent<CPTransform>()->GetPosition().x + m_horizontalPopUpOffset.x * m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentUpgrades,
																		popUpHPtr->GetComponent<CPTransform>()->GetPosition().y));

		// - Update how many upgrades the player has
		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentUpgrades += 1;
	}


	/*!***********************************************************************
	 \brief
	 For event subscription to decrease health when the player takes damage
	*************************************************************************/
	void DecreaseHealth()
	{
		if (GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->DecreaseHealth();
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to decrease ball when the player places one
	*************************************************************************/
	void DecreaseBalls()
	{
		if(GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->DecreaseBalls();
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase health when the player regens health
	*************************************************************************/
	void IncreaseHealth()
	{
		if (GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseHealth();
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase ball count when the ball expires
	*************************************************************************/
	void IncreaseBalls()
	{
		if (GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseBalls();
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max balls in PlayerHUD
	*************************************************************************/
	void IncreaseMaxHealth(int amount)
	{
		if (GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(amount);
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max health in PlayerHUD
	*************************************************************************/
	void IncreaseMaxBalls(int amount)
	{
		if (GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(amount);
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to add a new upgrade in PlayerHUD
	*************************************************************************/
	void AddNewUpgrade(UpgradeType upgrade)
	{
		if (GOMANAGER->FindGameObjectWithName("PlayerHUD"))
			GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->AddNewUpgrade(upgrade);
	}


}