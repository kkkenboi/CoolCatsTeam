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


namespace LB {

	/*!***********************************************************************
	\brief
	Start function to initialise the heart and ball display variables based on
	the current player's variables.
	*************************************************************************/
	void CPPSPlayerHUD::Start()
	{
		std::cout << "starter\n";

		// Initialise pointers to specific game objects
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_UpgradeManager = GOMANAGER->FindGameObjectWithName("Upgrade Manager");
		m_Mouse = GOMANAGER->FindGameObjectWithName("PlayerHUD");
		/*JSONSerializer::DeserializeFromFile("MouseHUD", *m_Mouse);*/

		// Create upgrade popups
		// - Consists of two game objects (Prefabs):
		// - 1. Main popup for the popup image and item name 
		// - 2. Flavour text
		// - We could add the item image again if we wanted to
		for (int i{1}; i < UpgradeType::MAXCOUNT; ++i)
		{
			GameObject* mainPopUp = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("UpgradePopUpHUD", *mainPopUp);
			mainPopUp->SetActive(false);

			GameObject* subFlavourText = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("UpgradePopUpTextHUD", *subFlavourText);
			subFlavourText->SetActive(false);

			// Since UpgradeType's based off enum, name them in that order
			switch (i)
			{
				// Set their names and text for each upgrade
				case SPLIT:
				{
					mainPopUp->SetName("SplitPopUp"), subFlavourText->SetName("SplitFlavourText");
					SetPopUpText("Split", ":^)", mainPopUp, subFlavourText);
					break;
				}
				case BOMB:
				{
					mainPopUp->SetName("BombPopUp"), subFlavourText->SetName("BombFlavourText");
					SetPopUpText("Bomb", "Explosive.", mainPopUp, subFlavourText);
					break;
				}
				case BIGBALL:
				{
					mainPopUp->SetName("BigBallPopUp"), subFlavourText->SetName("BigBallFlavourText");
					SetPopUpText("Big Balls", "They won't fit.", mainPopUp, subFlavourText);
					break;
				}
				case MOVESPEED:
				{
					mainPopUp->SetName("BOSPopUp"), subFlavourText->SetName("BOSFlavourText");
					SetPopUpText("Boots of Speed", "Can't touch this.", mainPopUp, subFlavourText);
					break;
				}
				case MOREHEALTH:
				{
					mainPopUp->SetName("HealthUpPopUp"), subFlavourText->SetName("HealthUpFlavourText");
					SetPopUpText("Health Up", "I thought it was a love potion.", mainPopUp, subFlavourText);
					break;
				}
				case MOREBALL:
				{
					mainPopUp->SetName("MoreBallsPopUp"), subFlavourText->SetName("MoreBallsFlavourText");
					SetPopUpText("More Balls", "More the merrier.", mainPopUp, subFlavourText);
					break;
				}
			}

			// Save to all popups container 
			m_totalUpgradePopUps.push_back(std::pair(mainPopUp, subFlavourText));
		}

		// Create game objects to display the health and balls
		if (!m_totalHeartDisplay.size()) // Fixes things
		{
			for (int i{ 1 }; i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth; i++)
			{
				GameObject* healthObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);
				Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
				healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (i - 1), startPos.y));
				//m_currentHealth = 2;
				// Set the texture for lost health
				if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
				{
					healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
						static_cast<int>(healthObject->GetComponent<CPRender>()->w),
						static_cast<int>(healthObject->GetComponent<CPRender>()->h));
				}

				m_totalHeartDisplay.push_back(healthObject);
			}

			for (int i{}; i < m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls; i++)
			{
				GameObject* ballObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("BallHUD", *ballObject);
				Vec2 startPos = ballObject->GetComponent<CPTransform>()->GetPosition();
				ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * i, startPos.y));

				m_totalBallsDisplay.push_back(ballObject);
			}
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

		// Update the MouseObject position to check if hovering UI through OnCollisionEnter
		Vec2<float> mousePos = INPUT->GetMousePos();

		mousePos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		mousePos.y = (mousePos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight()) * 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		m_Mouse->GetComponent<CPTransform>()->SetPosition(mousePos);
		
		// If there is a collision, based on the index given, set the position of the popup and toggleActive
		if (m_mouseHovering)
		{
			// For reference, generally x - 70 , y + 50 if main popup, y + 5 if flavour
			// main popup Image IMAGE POSITION
			// x 1800 y 300
			// main popup Image SPRITE SIZE 
			// 200 by 200 => x 1700 to 1900 => 1730 to 1870 => total x bound 140
			// main popup text TEXT POSITION
			// x 1730 y 355
			// flavour text TEXT POSITION
			// x 1730 y 305
			std::cout << "Mouse is hovering!!!\n";
			m_totalUpgradePopUps[m_currentPopUpIndex].first->SetActive(true);
			m_totalUpgradePopUps[m_currentPopUpIndex].second->SetActive(true);
		}
		else
		{
			m_totalUpgradePopUps[m_currentPopUpIndex].first->SetActive(false);
			m_totalUpgradePopUps[m_currentPopUpIndex].second->SetActive(false);
		}


		// Set mouse to not hover incase the mouse isn't hovering anymore
		m_mouseHovering = false;
	}

	void CPPSPlayerHUD::OnCollisionEnter(CollisionData colData)
	{
		std::cout << "Entering collision enter\n";
		if (colData.colliderOther->m_gameobj->GetComponent<CPRender>()->spriteSheetName == "Items")
		{
			std::cout << "Passing check\n";
			m_mouseHovering = true;
			m_currentPopUpIndex = static_cast<UpgradeType>(colData.colliderOther->m_gameobj->GetComponent<CPRender>()->spriteIndex - 1); // Minus 1 since the enum starts from 1
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
		if (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth >= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth)
		{
			--m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth;
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
			healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth + i - 1), startPos.y));
			//m_currentHealth = 2;
			// Set the texture for lost health
			if (i > m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentHealth)
			{
				healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
					static_cast<int>(healthObject->GetComponent<CPRender>()->w),
					static_cast<int>(healthObject->GetComponent<CPRender>()->h));
			}

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
			ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls + i), startPos.y));

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

		// - Set position on screen
		Vec2 upgradePosition = upgradeObject->GetComponent<CPTransform>()->GetPosition() - Vec2<float>(m_displayOffset.x * m_totalUpgradeDisplay.size(), 0.f);
		upgradeObject->GetComponent<CPTransform>()->SetPosition(upgradePosition);

		// - Set image and spritesheet + index
		upgradeObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Items"),
																static_cast<int>(upgradeObject->GetComponent<CPRender>()->w),
																static_cast<int>(upgradeObject->GetComponent<CPRender>()->h));

		upgradeObject->GetComponent<CPRender>()->spriteIndex = static_cast<int>(upgrade - 1); // Minus 1 since the enum starts from 1
		upgradeObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", static_cast<int>(upgrade - 1)); // Minus 1 since the enum starts from 1

		// Set the popup object(image) Position
		m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)].first->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, (upgradePosition.y + upgradeObject->GetComponent<CPRender>()->h / 2.f) + 100.f));
		// Set the popup text Position
		m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)].first->GetComponent<CPText>()->update_msg_pos(Vec2<float>((upgradePosition.x - upgradeObject->GetComponent<CPRender>()->w / 2.f) + 30.f, (upgradePosition.y + upgradeObject->GetComponent<CPRender>()->h / 2.f) + 160.f));
		// Set the flavour text Position
		m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)].second->GetComponent<CPText>()->update_msg_pos(Vec2<float>((upgradePosition.x - upgradeObject->GetComponent<CPRender>()->w / 2.f) + 30.f, (upgradePosition.y + upgradeObject->GetComponent<CPRender>()->h / 2.f) + 105.f));

		m_totalUpgradeDisplay.push_back(upgradeObject);
	}

	void CPPSPlayerHUD::SetPopUpText(std::string itemName, std::string flavourText, GameObject* mainPopUp, GameObject* subFlavourText)
	{
		std::string buffer = itemName;
		char itemText[200]{};

		// Set mainPopUp's text
		strcpy_s(itemText, sizeof(buffer), buffer.c_str());
		mainPopUp->GetComponent<CPText>()->set_msg(itemText);

		// Set subFlavourText's text
		buffer = flavourText;
		strcpy_s(itemText, sizeof(buffer), buffer.c_str());
		subFlavourText->GetComponent<CPText>()->set_msg(itemText);
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