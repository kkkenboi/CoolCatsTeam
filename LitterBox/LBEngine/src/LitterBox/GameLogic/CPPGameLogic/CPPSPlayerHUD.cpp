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

namespace LB {

	/*!***********************************************************************
	\brief
	Start function to initialise the heart and ball display variables based on
	the current player's variables.
	*************************************************************************/
	void CPPSPlayerHUD::Start()
	{
		// Create no. of hearts and balls based on the player's info
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "MainChar")
			{
				m_mainChar = gameObj;
			}
		}

		// Initialise health and balls values
		m_maxHealth = m_mainChar->GetComponent<CPPSPlayer>()->m_maxHealth;
		m_currentHealth = m_mainChar->GetComponent<CPPSPlayer>()->m_currentHealth;
		//std::cout << "Current Health: " << m_currentHealth << std::endl;
		m_maxBalls = m_mainChar->GetComponent<CPPSPlayer>()->m_maxBalls;
		m_currentBalls = m_maxBalls - m_mainChar->GetComponent<CPPSPlayer>()->m_currentBalls;

		// Fixes things
		if (!m_totalHeartDisplay.size())
		{
			// Create game objects to display the health and balls
			for (int i{ 1 }; i <= m_maxHealth; i++)
			{
				GameObject* healthObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);
				Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
				healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (i - 1), startPos.y));
				//m_currentHealth = 2;
				// Set the texture for lost health
				if (i > m_currentHealth)
				{
					healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
						static_cast<int>(healthObject->GetComponent<CPRender>()->w),
						static_cast<int>(healthObject->GetComponent<CPRender>()->h));
				}

				m_totalHeartDisplay.push_back(healthObject);
			}

			for (int i{}; i < m_maxBalls; i++)
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
		// HUD subcribes to player losing health/losing balls
		// 
		// If Player takes damage, decrement m_currentHealth, invokes OnHealthLoss<Bool> Event
		// Event calls the HUD function, decrementing the  decrements script's health, update HUD's

		if (m_decreaseHealth)
		{
			// Doing this for now to stop it from going beyond 0
			if (m_currentHealth)
			{
				--m_currentHealth;
				m_decreaseHealth = false;
			}
		}

		if (m_decreaseBalls)
		{
			// Balls doesn't need the same check as the health since there's its own check in CPPSPlayer
			--m_currentBalls;
			m_decreaseBalls = false;
		}

		for (size_t i{ 1 }; i <= m_maxHealth; ++i)
		{
			// Set the texture for lost health
			if (i > m_currentHealth)
			{
				m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"), 
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->w), 
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->h));
			}
			else if ( i <= m_currentHealth)
			{
				m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Heart"),
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->w),
																					static_cast<int>(m_totalHeartDisplay[i - 1]->GetComponent<CPRender>()->h));
			}
		}

		//std::cout << "Max Health: " << m_maxHealth << " " << "Current Health: " << m_currentHealth << std::endl;

		for (size_t i{ 1 }; i <= m_maxBalls; i++)
		{
			// Set the texture for lost health
			if (i > m_currentBalls)
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
		m_decreaseHealth = true;
	}

	/*!***********************************************************************
	 \brief
	 Sets a bool to decrease the HUD's ball to true
	*************************************************************************/
	void CPPSPlayerHUD::DecreaseBalls()
	{
		m_decreaseBalls = true;
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for current health in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseHealth()
	{
		if (m_currentHealth >= m_maxHealth)
		{
			++m_currentHealth;
		}
	}


	/*!***********************************************************************
	 \brief
	 Increases the member variable for current balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseBalls()
	{
		if (m_currentBalls <= m_maxBalls)
		{
			++m_currentBalls;
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
			healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (m_maxHealth + i - 1), startPos.y));
			//m_currentHealth = 2;
			// Set the texture for lost health
			if (i > m_currentHealth)
			{
				healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
					static_cast<int>(healthObject->GetComponent<CPRender>()->w),
					static_cast<int>(healthObject->GetComponent<CPRender>()->h));
			}

			m_totalHeartDisplay.push_back(healthObject);
		}

		m_maxHealth += amount;
		m_currentHealth += amount;
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
			ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + m_displayOffset.x * (m_maxBalls + i), startPos.y));

			m_totalBallsDisplay.push_back(ballObject);
		}

		m_maxBalls += amount;
		m_currentBalls += amount;
		m_mainChar->GetComponent<CPPSPlayer>()->m_maxBalls += amount;
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