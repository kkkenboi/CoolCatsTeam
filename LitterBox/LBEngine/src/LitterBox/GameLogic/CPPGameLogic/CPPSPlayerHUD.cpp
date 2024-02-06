#include "CPPSPlayerHUD.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/GameLogic/CPPGameLogic/CPPSPlayer.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB {
	//Vec2 heartDisplayStartingPos   { 126.f, 960.f };
	//Vec2 ballDisplayStartingPos    { 126.f, 830.f };
	Vec2 upgradeDisplayStartingPos { 1150.f, 110.f };

	//Vec2 heartDisplayScale  { 0.3f, 0.3f };
	//Vec2 ballDisplayScale   { 0.3f, 0.3f };
	//Vec2 upgradeDisplayScale{ 0.3f, 0.3f };
	Vec2 displayScale{ 1.f, 1.f };

	//Vec2 heartDisplayOffset{ 130.f, 130.f };
	//Vec2 ballDisplayOffset{ 130.f, 130.f };
	//Vec2 upgradeDisplayOffset{ 130.f, 130.f };
	Vec2 displayOffset { 130.f, 130.f };

	void CPPSPlayerHUD::Start()
	{
		// Create no. of hearts and balls based on the player's info
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "MainChar")
			{
				mainChar = gameObj;
			}
		}

		// Initialise health and balls values
		m_maxHealth = mainChar->GetComponent<CPPSPlayer>()->m_maxHealth;
		m_currentHealth = mainChar->GetComponent<CPPSPlayer>()->m_currentHealth;
		//std::cout << "Current Health: " << m_currentHealth << std::endl;
		m_maxBalls = mainChar->GetComponent<CPPSPlayer>()->m_maxBalls;
		m_currentBalls = m_maxBalls - mainChar->GetComponent<CPPSPlayer>()->m_currentBalls;

		// Fixes things
		if (!m_TotalHeartDisplay.size())
		{
			// Create game objects to display the health and balls
			for (int i{ 1 }; i <= m_maxHealth; i++)
			{
				GameObject* healthObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("HeartHUD", *healthObject);
				Vec2 startPos = healthObject->GetComponent<CPTransform>()->GetPosition();
				healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + displayOffset.x * (i - 1), startPos.y));
				//m_currentHealth = 2;
				// Set the texture for lost health
				if (i > m_currentHealth)
				{
					healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
						static_cast<int>(healthObject->GetComponent<CPRender>()->w),
						static_cast<int>(healthObject->GetComponent<CPRender>()->h));
				}

				m_TotalHeartDisplay.push_back(healthObject);
			}

			for (int i{}; i < m_maxBalls; i++)
			{
				GameObject* ballObject = FACTORY->SpawnGameObject();
				JSONSerializer::DeserializeFromFile("BallHUD", *ballObject);
				Vec2 startPos = ballObject->GetComponent<CPTransform>()->GetPosition();
				ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + displayOffset.x * i, startPos.y));

				m_TotalBallsDisplay.push_back(ballObject);
			}
		}
	}

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
				m_TotalHeartDisplay[i - 1]->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"), 
																					static_cast<int>(m_TotalHeartDisplay[i - 1]->GetComponent<CPRender>()->w), 
																					static_cast<int>(m_TotalHeartDisplay[i - 1]->GetComponent<CPRender>()->h));
			}
			else if ( i <= m_currentHealth)
			{
				m_TotalHeartDisplay[i - 1]->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Heart"),
																					static_cast<int>(m_TotalHeartDisplay[i - 1]->GetComponent<CPRender>()->w),
																					static_cast<int>(m_TotalHeartDisplay[i - 1]->GetComponent<CPRender>()->h));
			}
		}

		//std::cout << "Max Health: " << m_maxHealth << " " << "Current Health: " << m_currentHealth << std::endl;

		for (size_t i{ 1 }; i <= m_maxBalls; i++)
		{
			// Set the texture for lost health
			if (i > m_currentBalls)
			{
				m_TotalBallsDisplay[i - 1]->GetComponent<CPRender>()->ToggleActive(false);
			}
			else
			{
				m_TotalBallsDisplay[i - 1]->GetComponent<CPRender>()->ToggleActive(true);
			}
		}
	}

	void CPPSPlayerHUD::Destroy()
	{

	}

	/*!***********************************************************************
	 \brief
	 Decreases the member variable for current health in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::DecreaseHealth()
	{
		m_decreaseHealth = true;
		//if (m_currentHealth)
		//{
		//	--m_currentHealth;
		//}
	}

	/*!***********************************************************************
	 \brief
	 Decreases the member variable for current balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::DecreaseBalls()
	{
		m_decreaseBalls = true;

		//if (m_currentBalls)
		//{
		//	--m_currentBalls;
		//}
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
			healthObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + displayOffset.x * (m_maxHealth + i - 1), startPos.y));
			//m_currentHealth = 2;
			// Set the texture for lost health
			if (i > m_currentHealth)
			{
				healthObject->GetComponent<CPRender>()->UpdateTexture(LB::ASSETMANAGER->GetTextureUnit("Broken Heart"),
					static_cast<int>(healthObject->GetComponent<CPRender>()->w),
					static_cast<int>(healthObject->GetComponent<CPRender>()->h));
			}

			m_TotalHeartDisplay.push_back(healthObject);
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
			ballObject->GetComponent<CPTransform>()->SetPosition(Vec2<float>(startPos.x + displayOffset.x * (m_maxBalls + i), startPos.y));

			m_TotalBallsDisplay.push_back(ballObject);
		}

		m_maxBalls += amount;
		m_currentBalls += amount;
		mainChar->GetComponent<CPPSPlayer>()->m_maxBalls += amount;
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