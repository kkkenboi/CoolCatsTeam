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
#include "CPPSUpgradeManager.h"


namespace LB {

	/*!***********************************************************************
	\brief
	Start function to initialise the heart and ball display variables based on
	the current player's variables.
	*************************************************************************/
	void CPPSPlayerHUD::Start()
	{
		// Create no. of hearts and balls based on the player's info
		m_GameManager = GOMANAGER->FindGameObjectWithName("GameManager");
		m_UpgradeManager = GOMANAGER->FindGameObjectWithName("Upgrade Manager");

		// Fixes things
		if (!m_totalHeartDisplay.size())
		{
			// Create game objects to display the health and balls
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
	}

	/*!***********************************************************************
	\brief
	Update will reference the player and update the HUD if there are any changes
	such as the health and the balls.
	*************************************************************************/
	void CPPSPlayerHUD::Update()
	{
		std::cout << m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerCurrentBalls << " " << m_GameManager->GetComponent<CPPSGameManager>()->m_PlayerMaxBalls << std::endl;

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

}