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
			verticalPopUp->SetActive(false);

			GameObject* horizontalPopUp = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("HorizontalPopUp", *horizontalPopUp);
			horizontalPopUp->SetActive(false);

			// Since UpgradeType's based off enum, name them in that order
			switch (i)
			{
				// Set their names and text for each upgrade
				case SPLIT:
				{
					verticalPopUp->SetName("SplitVPopUp"), horizontalPopUp->SetName("SplitHPopUp");
					verticalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					horizontalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					break;
				}
				case BOMB:
				{
					verticalPopUp->SetName("GolfBombVPopUp"), horizontalPopUp->SetName("GolfBombHPopUp");
					verticalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					horizontalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					break;
				}
				case BIGBALL:
				{
					verticalPopUp->SetName("BiggieBallVPopUp"), horizontalPopUp->SetName("BiggieBallHPopUp");
					verticalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					horizontalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					break;
				}
				case MOVESPEED:
				{
					verticalPopUp->SetName("SpeedsterVPopUp"), horizontalPopUp->SetName("SpeedsterHPopUp");
					verticalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					horizontalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					break;
				}
				case MOREHEALTH:
				{
					verticalPopUp->SetName("ExtraLivesVPopUp"), horizontalPopUp->SetName("ExtraLivesHPopUp");
					verticalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					horizontalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					break;
				}
				case MOREBALL:
				{
					verticalPopUp->SetName("BagOfHoldingVPopUp"), horizontalPopUp->SetName("BagOfHoldingHPopUp");
					verticalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					horizontalPopUp->GetComponent<CPRender>()->spriteIndex = 0; // TO BE EDITED WHEN SPRITESHEET IS RECEIVED
					break;
				}
			}

			// Save to all popups container 
			m_totalUpgradePopUps.push_back(std::pair(verticalPopUp, horizontalPopUp));
		}

		// Create game objects to display the health and balls
		for (int i{ 1 }; i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth; ++i)
		{
			GameObject* healthObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);
			Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
			healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (i - 1), startPos.y));

			// Set the texture for lost health
			if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
					static_cast<int>(healthObject->GetComponent<CPRender>()->w),
					static_cast<int>(healthObject->GetComponent<CPRender>()->h));
			}
			m_totalHeartDisplay.push_back(healthObject);
		}

		for (int i{}; i < m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls; ++i)
		{
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("BallHUD", *ballObject);
			Vec2 startPos = ballObject->GetComponent<CPTransform>()->GetPosition();
			ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * i, startPos.y));

			m_totalBallsDisplay.push_back(ballObject);
		}

		// - To know when new upgrades are obtained
		m_UpgradeManager->GetComponent<CPPSUpgradeManager>()->onNewUpgrade.Subscribe(LB::AddNewUpgrade);
	}

	/*!***********************************************************************
	\brief
	Update will reference the player and update the HUD if there are any changes
	such as the health and the balls.
	*************************************************************************/
	void CPPSPlayerHUD::Update()
	{
		for (size_t i{ 1 }; i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth; ++i)
		{
			// Set the texture for lost health
			if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"), 
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->w), 
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->h));

			}
			else if ( i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Heart"),
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->w),
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->h));
			}
			// Update the location of the heart based on the camera follow
			//std::cout << m_totalHeartDisplay[i - 1]->GetComponent<CPTransform>()->GetParent()->gameObj->GetName() << std::endl;
		}

		for (size_t i{ 1 }; i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls; ++i)
		{
			// Set the texture for lost health
			if (i > static_cast<size_t>(m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls - m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls))
			{
				m_totalBallsDisplay[i - 1]->GetComponent<CPRender>()->ToggleActive(false);
			}
			else
			{
				m_totalBallsDisplay[i - 1]->GetComponent<CPRender>()->ToggleActive(true);
			}
		}
		
		// If there is a collision, based on the index given, set the position of the popup and toggleActive
		for (size_t i = 0; i < m_totalUpgradePopUps.size(); ++i)
		{
			if (i == static_cast<size_t>(m_currentPopUpIndex))
			{
				m_totalUpgradePopUps[i].first->SetActive(m_mouseHoverWorld);
				m_totalUpgradePopUps[i].second->SetActive(m_mouseHoverUI);
				//if (m_mouseHoverWorld)
				//{
				//	m_totalUpgradePopUps[i].first->SetActive(true);
				//}
				//else
				//{
				//	m_totalUpgradePopUps[i].first->SetActive(false);
				//}

				//if (m_mouseHoverUI)
				//{
				//	m_totalUpgradePopUps[i].second->SetActive(true);
				//}
				//else
				//{
				//	m_totalUpgradePopUps[i].second->SetActive(false);
				//}
			}
			else
			{
				m_totalUpgradePopUps[i].first->SetActive(false);
				m_totalUpgradePopUps[i].second->SetActive(false);
			}
		}

		// Set mouse to not hover incase the mouse isn't hovering anymore
		// - This is very bad fix to make it remove because the hovering still occurs
		// - even if it's being collided with other stuff that's not the UpgradeHUD and 
		// - my brain literally cannot right now, I WANT TO SLEEP PLEASE FORGIVE ME
		//if (!m_MouseUI->GetComponent<CPCollider>()->m_collided)
		//{
		//	m_mouseHovering = false;
		//	m_mouseHovering2 = false;
		//	for (size_t i = 0; i < m_totalUpgradePopUps.size(); ++i)
		//	{
		//		m_totalUpgradePopUps[i].first->SetActive(false);
		//		m_totalUpgradePopUps[i].second->SetActive(false);
		//	}
		//}
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
		// Create game objects to display the health and balls
		for (int i{ 1 }; i <= amount; i++)
		{
			GameObject* healthObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);
			Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
			//Vec2 startPos = Vec2<float>(-(WINDOWSSYSTEM->GetScreenWidth() / 4.0f), WINDOWSSYSTEM->GetScreenHeight() / 4.0f - m_displayOffset.y);
			healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth + i - 1), startPos.y));
			//m_currentHealth = 2;
			// Set the texture for lost health
			if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
					static_cast<int>(healthObject->GetComponent<CPRender>()->w),
					static_cast<int>(healthObject->GetComponent<CPRender>()->h));
			}
			//healthObject->GetComponent<CPTransform>()->SetParent(GOMANAGER->FindGameObjectWithName("CameraFollow")->GetComponent<CPTransform>());

			m_totalHeartDisplay.push_back(healthObject);
		}

		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth += amount;
		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth += amount;
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for max balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseMaxBalls(int amount)
	{
		for (int i{}; i < amount; i++)
		{
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("BallHUD", *ballObject);
			Vec2 startPos = ballObject->GetComponent<CPTransform>()->GetPosition();
			//Vec2 startPos = Vec2<float>(-(WINDOWSSYSTEM->GetScreenWidth() / 4.0f), WINDOWSSYSTEM->GetScreenHeight() / 4.0f - m_displayOffset.y * 2.0f);
			ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls + i), startPos.y));
			//ballObject->GetComponent<CPTransform>()->SetParent(GOMANAGER->FindGameObjectWithName("CameraFollow")->GetComponent<CPTransform>());

			m_totalBallsDisplay.push_back(ballObject);
		}

		m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls += amount;
		//m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls += amount;
	}

	/*!***********************************************************************
	 \brief
	 To add a new upgrade the player has for the upgrade display in PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::AddNewUpgrade(UpgradeType upgrade)
	{
		// Create a new upgrade display for HUD
		GameObject* upgradeObject = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("UpgradeHUD", *upgradeObject);
		
		// - Set position on screen for the upgrade and the horizontal popup
		upgradeObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradeObject->GetComponent<CPTransform>()->GetPosition().x - m_displayOffset.x * m_totalUpgradeDisplay.size(), 
																			upgradeObject->GetComponent<CPTransform>()->GetPosition().y));

		m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)].second->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradeObject->GetComponent<CPTransform>()->GetPosition().x, upgradeObject->GetComponent<CPTransform>()->GetPosition().y + 100.f));

		// - Set image and spritesheet + index
		upgradeObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Items"),
																static_cast<int>(upgradeObject->GetComponent<CPRender>()->w),
																static_cast<int>(upgradeObject->GetComponent<CPRender>()->h));

		upgradeObject->GetComponent<CPRender>()->spriteIndex = static_cast<int>(upgrade - 1); // Minus 1 since the enum starts from 1
		upgradeObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", static_cast<int>(upgrade - 1)); // Minus 1 since the enum starts from 1

		m_totalUpgradeDisplay.push_back(upgradeObject);
	}


	/*!***********************************************************************
	 \brief
	 For event subscription to decrease health when the player takes damage
	*************************************************************************/
	void DecreaseHealth()
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->DecreaseHealth();
				break;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to decrease ball when the player places one
	*************************************************************************/
	void DecreaseBalls()
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->DecreaseBalls();
				break;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase health when the player regens health
	*************************************************************************/
	void IncreaseHealth()
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseHealth();
				break;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase ball count when the ball expires
	*************************************************************************/
	void IncreaseBalls()
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseBalls();
				break;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max balls in PlayerHUD
	*************************************************************************/
	void IncreaseMaxHealth(int amount)
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(amount);
				break;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max health in PlayerHUD
	*************************************************************************/
	void IncreaseMaxBalls(int amount)
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(amount);
				break;
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to add a new upgrade in PlayerHUD
	*************************************************************************/
	void AddNewUpgrade(UpgradeType upgrade)
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->AddNewUpgrade(upgrade);
	}


}