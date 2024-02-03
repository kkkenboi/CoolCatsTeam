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
				healthObject->GetComponent<CPRender>()->texture = ASSETMANAGER->Textures[ASSETMANAGER->assetMap["Broken Heart"]].second;
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

	void CPPSPlayerHUD::Update()
	{
		// HUD subcribes to player losing health/losing balls
		// 
		// If Player takes damage, decrement m_currentHealth, invokes OnHealthLoss<Bool> Event
		// Event calls the HUD function, decrementing the  decrements script's health, update HUD's

		for (size_t i{ 1 }; i <= m_maxHealth; i++)
		{
			// Set the texture for lost health
			if (i > m_currentHealth)
			{
				m_TotalHeartDisplay[i-1]->GetComponent<CPRender>()->texture = ASSETMANAGER->Textures[ASSETMANAGER->assetMap["Broken Heart"]].second;
			}
		}

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
		--m_currentHealth;
	}

	/*!***********************************************************************
	 \brief
	 Decreases the member variable for current balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::DecreaseBalls()
	{
		--m_currentBalls;
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for current health in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseHealth()
	{
		++m_currentHealth;
	}

	/*!***********************************************************************
	 \brief
	 Increases the member variable for current balls in the PlayerHUD
	*************************************************************************/
	void CPPSPlayerHUD::IncreaseBalls()
	{
		++m_currentBalls;
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
			}
		}
	}

	/*!***********************************************************************
	 \brief
	 For event subscription to increaase ball when the ball expires
	*************************************************************************/
	void IncreaseBalls()
	{
		for (GameObject* gameObj : GOMANAGER->GetGameObjects())
		{
			// Find out which object is the player
			if (gameObj->GetName() == "PlayerHUD")
			{
				gameObj->GetComponent<CPPSPlayerHUD>()->IncreaseBalls();
			}
		}
	}

}