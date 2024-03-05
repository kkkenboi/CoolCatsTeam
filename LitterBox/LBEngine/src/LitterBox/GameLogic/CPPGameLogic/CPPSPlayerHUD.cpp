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
		// Initialise pointers to specific game objects
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_UpgradeManager = GOMANAGER->FindGameObjectWithName("Upgrade Manager");
		m_Mouse = GOMANAGER->FindGameObjectWithName("PlayerHUD");

		// Create upgrade popups
		// - Consists of three game objects (Prefabs):
		// - 1. Main popup for the popup image and item name 
		// - 2. Info text to explain the upgrade
		// - 3. Flavour text
		// - We could add the item image again if we wanted to
		for (int i{1}; i < UpgradeType::MAXCOUNT; ++i)
		{
			GameObject* mainPopUp = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("UpgradePopUpHUD", *mainPopUp);
			mainPopUp->SetActive(false);

			GameObject* infoObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("UpgradePopUpTextHUD", *infoObject);
			infoObject->SetActive(false);

			GameObject* flavourObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("UpgradePopUpFlavourTextHUD", *flavourObject);
			flavourObject->SetActive(false);

			// Since UpgradeType's based off enum, name them in that order
			switch (i)
			{
				// Set their names and text for each upgrade
				case SPLIT:
				{
					mainPopUp->SetName("SplitPopUp"), infoObject->SetName("SplitInfoText"), flavourObject->SetName("SplitFlavourText");
					infoObject->GetComponent<CPRender>()->spriteIndex = i - 1;
					infoObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", infoObject->GetComponent<CPRender>()->spriteIndex);
					SetPopUpText("Split", "   Ricochets to     another enemy.", "Curveball.", mainPopUp, infoObject, flavourObject);
					break;
				}
				case BOMB:
				{
					mainPopUp->SetName("GolfBombPopUp"), infoObject->SetName("GolfBombInfoText"), flavourObject->SetName("GolfBombFlavourText");
					infoObject->GetComponent<CPRender>()->spriteIndex = i - 1;
					infoObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", infoObject->GetComponent<CPRender>()->spriteIndex);
					SetPopUpText("Golf Bomb", "    Explodes on             contact", "    Do you hear      that psst sound?", mainPopUp, infoObject, flavourObject);
					break;
				}
				case BIGBALL:
				{
					mainPopUp->SetName("BiggieBallPopUp"), infoObject->SetName("BiggieBallInfoText"), flavourObject->SetName("BiggieBallFlavourText");
					infoObject->GetComponent<CPRender>()->spriteIndex = i - 1;
					infoObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", infoObject->GetComponent<CPRender>()->spriteIndex);
					SetPopUpText("Biggie Ball", "Increases size of    the golf ball", "  Even golf balls       need protein       powder to grow.", mainPopUp, infoObject, flavourObject);
					break;
				}
				case MOVESPEED:
				{
					mainPopUp->SetName("SpeedsterPopUp"), infoObject->SetName("SpeedsterInfoText"), flavourObject->SetName("SpeedsterFlavourText");
					infoObject->GetComponent<CPRender>()->spriteIndex = i - 1;
					infoObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", infoObject->GetComponent<CPRender>()->spriteIndex);
					SetPopUpText("Speedster", "Move quicker in    all directions", "Shoes helps in the         long run.", mainPopUp, infoObject, flavourObject);
					break;
				}
				case MOREHEALTH:
				{
					mainPopUp->SetName("ExtraLivesPopUp"), infoObject->SetName("ExtraLivesInfoText"), flavourObject->SetName("ExtraLivesFlavourText");
					infoObject->GetComponent<CPRender>()->spriteIndex = i - 1;
					infoObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", infoObject->GetComponent<CPRender>()->spriteIndex);
					SetPopUpText("Extra Lives", "Increase lives              3 >> 6", "Nine lives are             a thing?", mainPopUp, infoObject, flavourObject);
					break;
				}
				case MOREBALL:
				{
					mainPopUp->SetName("BagOfHoldingPopUp"), infoObject->SetName("BagOfHoldingInfoText"), flavourObject->SetName("BagOfHoldingFlavourText");
					infoObject->GetComponent<CPRender>()->spriteIndex = i - 1;
					infoObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", infoObject->GetComponent<CPRender>()->spriteIndex);
					// Set the text Scale
					SetPopUpText("Bag of Balls", "Golfball capacity          3 >> 6", "Looks bigger on        the inside.", mainPopUp, infoObject, flavourObject);
					//SetPopUpText("More Balls", "Golfball capacity           3 > 5", "Meow the merrier.", mainPopUp, infoObject, flavourObject);
					break;
				}
			}

			// Save to all popups container 
			m_totalUpgradePopUps.push_back(std::tuple(mainPopUp, infoObject, flavourObject));
		}

		// Create game objects to display the health and balls
		//if (!m_totalHeartDisplay.size()) // Fixes things
		//{
			for (int i{ 1 }; i <= m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxHealth; i++)
			{
				GameObject* healthObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);
				Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
				//Vec2 startPos = Vec2<float>(-(WINDOWSSYSTEM->GetScreenWidth() / 4.0f), WINDOWSSYSTEM->GetScreenHeight() / 4.0f - m_displayOffset.y);
				healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (i - 1), startPos.y));
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

			for (int i{}; i < m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls; i++)
			{
				GameObject* ballObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("BallHUD", *ballObject);
				Vec2 startPos = ballObject->GetComponent<CPTransform>()->GetPosition();
				//Vec2 startPos = Vec2<float>(-(WINDOWSSYSTEM->GetScreenWidth() / 4.0f), WINDOWSSYSTEM->GetScreenHeight() / 4.0f - m_displayOffset.y * 2.0f);
				ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * i, startPos.y));
				//ballObject->GetComponent<CPTransform>()->SetParent(GOMANAGER->FindGameObjectWithName("CameraFollow")->GetComponent<CPTransform>());

				m_totalBallsDisplay.push_back(ballObject);
			}
		//}

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

		// Update the MouseObject position to check if hovering UI through OnCollisionEnter
		Vec2<float> mousePos = INPUT->GetMousePos();

		mousePos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		mousePos.y = (mousePos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight()) * 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		m_Mouse->GetComponent<CPTransform>()->SetPosition(mousePos);

		//m_mouseHovering = m_Mouse->GetComponent<CPCollider>()->m_collided;
		
		// If there is a collision, based on the index given, set the position of the popup and toggleActive
		for (size_t i = 0; i < m_totalUpgradePopUps.size(); ++i)
		{
			if (i == static_cast<size_t>(m_currentPopUpIndex))
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
				std::get<0>(m_totalUpgradePopUps[i])->SetActive(m_mouseHovering);
				std::get<1>(m_totalUpgradePopUps[i])->SetActive(m_mouseHovering);
				std::get<2>(m_totalUpgradePopUps[i])->SetActive(m_mouseHovering);
			}
			else
			{
				std::get<0>(m_totalUpgradePopUps[i])->SetActive(false);
				std::get<1>(m_totalUpgradePopUps[i])->SetActive(false);
				std::get<2>(m_totalUpgradePopUps[i])->SetActive(false);
			}
		}

		// Set mouse to not hover incase the mouse isn't hovering anymore
		// - This is very bad fix to make it remove because the hovering still occurs
		// - even if it's being collided with other stuff that's not the UpgradeHUD and 
		// - my brain literally cannot right now, I WANT TO SLEEP PLEASE FORGIVE ME
		if (!m_Mouse->GetComponent<CPCollider>()->m_collided)
		{
			m_mouseHovering = false;
			for (size_t i = 0; i < m_totalUpgradePopUps.size(); ++i)
			{
				std::get<0>(m_totalUpgradePopUps[i])->SetActive(false);
				std::get<1>(m_totalUpgradePopUps[i])->SetActive(false);
				std::get<2>(m_totalUpgradePopUps[i])->SetActive(false);
			}
		}
	}

	void CPPSPlayerHUD::OnCollisionEnter(CollisionData colData)
	{
		if (colData.colliderOther->m_gameobj->GetName() == "UpgradeHUD")
		{
			m_mouseHovering = true;
			m_currentPopUpIndex = static_cast<UpgradeType>(colData.colliderOther->m_gameobj->GetComponent<CPRender>()->spriteIndex);
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

		// - Set position on screen
		Vec2 upgradePosition = upgradeObject->GetComponent<CPTransform>()->GetPosition() - Vec2<float>(m_displayOffset.x * m_totalUpgradeDisplay.size(), 0.f);
		upgradeObject->GetComponent<CPTransform>()->SetPosition(upgradePosition);

		// - Set image and spritesheet + index
		upgradeObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Items"),
																static_cast<int>(upgradeObject->GetComponent<CPRender>()->w),
																static_cast<int>(upgradeObject->GetComponent<CPRender>()->h));

		upgradeObject->GetComponent<CPRender>()->spriteIndex = static_cast<int>(upgrade - 1); // Minus 1 since the enum starts from 1
		upgradeObject->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", static_cast<int>(upgrade - 1)); // Minus 1 since the enum starts from 1

		// Updating upgradePosition which is now used for the popups, this is going to be set to 200.f
		upgradePosition.y = upgradePosition.y + upgradeObject->GetComponent<CPRender>()->h / 2.f;
		// Hardcoding because I have to with the text system
		switch (upgrade)
		{
			// Set their names and text for each upgrade
			// Reference 
			// - mainPopUp x = 1800.00, y = 300.00
			// - image pos x = 1800.00, y = 360.00
			case SPLIT:
			{
				// Set the mainPopup image Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 100.f));
				// Set the mainPopup text Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 29.f, upgradePosition.y + 226.f));
				// Set the infoObject image Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 160.f));
				// Set the infoObject text Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 67.f, upgradePosition.y + 60.f));
				// Set the flavourObject text Position
				std::get<2>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 40.f, upgradePosition.y));
				break;
			}
			case BOMB:
			{
				// Set the mainPopup image Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 100.f));
				// Set the mainPopup text Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 57.f, upgradePosition.y + 226.f));
				// Set the infoObject image Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 160.f));
				// Set the infoObject text Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 70.f, upgradePosition.y + 60.f));
				// Set the flavourObject text Position ----------------
				std::get<2>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 63.f, upgradePosition.y));
				break;
			}
			case BIGBALL:
			{
				// Set the mainPopup image Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 100.f));
				// Set the mainPopup text Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 57.f, upgradePosition.y + 226.f));
				// Set the infoObject image Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 160.f));
				// Set the infoObject text Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 69.f, upgradePosition.y + 60.f));
				// Set the flavourObject text Position
				std::get<2>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 63.f, upgradePosition.y));
				break;
			}
			case MOVESPEED:
			{
				// Set the mainPopup image Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 100.f));
				// Set the mainPopup text Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 55.f, upgradePosition.y + 226.f));
				// Set the infoObject image Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 160.f));
				// Set the infoObject text Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 69.f, upgradePosition.y + 60.f));
				// Set the flavourObject text Position
				std::get<2>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 68.f, upgradePosition.y));
				break;
			}
			case MOREHEALTH:
			{
				// Set the mainPopup image Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 100.f));
				// Set the mainPopup text Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 65.f, upgradePosition.y + 226.f));
				// Set the infoObject image Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 160.f));
				// Set the infoObject text Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 58.f, upgradePosition.y + 60.f));
				// Set the flavourObject text Position
				std::get<2>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 55.f, upgradePosition.y));
				break;
			}
			case MOREBALL:
			{
				// Set the mainPopup image Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 100.f));
				// Set the mainPopup text Position
				std::get<0>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 63.f, upgradePosition.y + 227.f));
				// Set the infoObject image Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPTransform>()->SetPosition(Vec2<float>(upgradePosition.x, upgradePosition.y + 160.f));
				// Set the infoObject text Position
				std::get<1>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 68.f, upgradePosition.y + 60.f));
				// Set the flavourObject text Position
				std::get<2>(m_totalUpgradePopUps[static_cast<size_t>(upgrade - 1)])->GetComponent<CPText>()->update_msg_pos(Vec2<float>(upgradePosition.x - 60.f, upgradePosition.y));
				break;
			}
		}

		m_totalUpgradeDisplay.push_back(upgradeObject);
	}

	void CPPSPlayerHUD::SetPopUpText(std::string itemName, std::string itemInfo, std::string flavourText, GameObject* mainPopUp, GameObject* infoObject, GameObject* flavourObject)
	{
		std::string buffer = itemName;
		char itemText[500]{};

		// Set mainPopUp's text
		std::copy(buffer.begin(), buffer.end(), itemText);
		itemText[buffer.size()] = '\0'; // Null-terminate the string
		mainPopUp->GetComponent<CPText>()->set_msg(itemText);

		// Set infoObject's text
		buffer.clear();
		buffer = itemInfo;
		std::copy(buffer.begin(), buffer.end(), itemText);
		itemText[buffer.size()] = '\0'; // Null-terminate the string
		infoObject->GetComponent<CPText>()->set_msg(itemText);

		// Set flavourObject's text
		buffer.clear();
		buffer = flavourText;
		std::copy(buffer.begin(), buffer.end(), itemText);
		itemText[buffer.size()] = '\0'; // Null-terminate the string
		flavourObject->GetComponent<CPText>()->set_msg(itemText);
	}


	/*!***********************************************************************
	 \brief
	 For event subscription to decrease health when the player takes damage
	*************************************************************************/
	void DecreaseHealth()
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->DecreaseHealth();
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the player
		//	if (gameObj->GetName() == "PlayerHUD")
		//	{
		//		gameObj->GetComponent<CPPSPlayerHUD>()->DecreaseHealth();
		//		break;
		//	}
		//}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to decrease ball when the player places one
	*************************************************************************/
	void DecreaseBalls()
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->DecreaseBalls();
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the player
		//	if (gameObj->GetName() == "PlayerHUD")
		//	{
		//		gameObj->GetComponent<CPPSPlayerHUD>()->DecreaseBalls();
		//		break;
		//	}
		//}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase health when the player regens health
	*************************************************************************/
	void IncreaseHealth()
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseHealth();
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the player
		//	if (gameObj->GetName() == "PlayerHUD")
		//	{
		//		gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseHealth();
		//		break;
		//	}
		//}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase ball count when the ball expires
	*************************************************************************/
	void IncreaseBalls()
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseBalls();
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the player
		//	if (gameObj->GetName() == "PlayerHUD")
		//	{
		//		gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseBalls();
		//		break;
		//	}
		//}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max balls in PlayerHUD
	*************************************************************************/
	void IncreaseMaxHealth(int amount)
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(amount);
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the player
		//	if (gameObj->GetName() == "PlayerHUD")
		//	{
		//		gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseMaxHealth(amount);
		//		break;
		//	}
		//}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increase the max health in PlayerHUD
	*************************************************************************/
	void IncreaseMaxBalls(int amount)
	{
		GOMANAGER->FindGameObjectWithName("PlayerHUD")->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(amount);
		//for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		//{
		//	// Find out which object is the player
		//	if (gameObj->GetName() == "PlayerHUD")
		//	{
		//		gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseMaxBalls(amount);
		//		break;
		//	}
		//}
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