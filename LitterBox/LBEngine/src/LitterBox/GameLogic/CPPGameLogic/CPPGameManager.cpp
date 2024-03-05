
/*!************************************************************************
 * \file				CPPGameManager.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2451A
 * \date				09/02/2024
 * \brief 				This source file contains the function definitions for the 
 * 						game manager that handles the spawning of the enemies through
 * 						the AI Director
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPGameManager.h"
#include "LitterBox/Engine/Input.h"
#include "CPPSUpgradeManager.h"
#include <random>
#include "CPPSPlayerGolfBall.h"
#include "CPPSBaseGolfBall.h"
namespace LB
{
	void CPPSGameManager::Start()
	{
		// Initialising player values
		m_PlayerMaxHealth = 3;
		m_PlayerCurrentHealth = 3;
		m_PlayerMaxBalls = 3;
		m_PlayerCurrentBalls = 0;
		m_PlayerWalkSpeed = 950.0f;
		m_PlayerMaxSpeed = 1750.0f;
		m_PlayerArbitraryFriction = 0.95f;

		//We also need to grab the crowdTexture
		//By default, the render is set active false
		crowdTexture = GOMANAGER->FindGameObjectWithName("CrowdTextureObject");
		//we also wanna cache the position of the UI so we can set it back later
		cachedCrowdPos = crowdTexture->GetComponent<CPTransform>()->GetPosition();

		//Damn scuffed way of doing this but we're adding the function ptr and cost to spawn
		//into a list
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnChaserEnemy, 2));
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnMageEnemy, 5));
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnChargerEnemy, 8));

		//For the first level we just make it such that it's always 2 melee enemies
		if (currentWave == 1) 
		{
			SpawnCredits = 4;
			GenerateWave();
			GameStart = true;
		}
	}

	void CPPSGameManager::Update()
	{
		//Mouse input stuff
		mouse_pos = INPUT->GetMousePos();
		mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
		mouse_pos.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		mouse_pos.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();

		////Manually Spawn Chaser Enemy
		if (INPUT->IsKeyTriggered(KeyCode::KEY_C))
		{
			SpawnChaserEnemy();
			//SpawnRandomEnemy();
		}

		////Manually Spawn Mage Enemy
		if (INPUT->IsKeyTriggered(KeyCode::KEY_M))
		{
			SpawnMageEnemy();
		}
		if (INPUT->IsKeyTriggered(KeyCode::KEY_G))
		{
			GenerateWave();
			GameStart = true;
		}
		//Test function to see if the remove gameobject code works
		//You have to comment out the ball's canDestroy code in order for this
		//to not crash the game
		if (INPUT->IsKeyTriggered(KeyCode::KEY_V))
		{
			GameObject* ballObject = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile("ball", *ballObject);
			GOMANAGER->RemoveGameObject(ballObject, 2.f);
		}

		if (INPUT->IsKeyTriggered(KeyCode::KEY_P))
		{
			SpawnChargerEnemy();
		}
		////Really really really scuffed way of doing this
		//if (currentEnemyCount == 0 && GameStart && UpgradePicked)
		//{
		//	//Spawn Upgrades, Do level transition blablabla
		//	currentWave++;
		//	GenerateWave();
		//	UpgradePicked = false;
		//}
		if (currentEnemyCount == 0 && GameStart && !UpgradeSpawned)
		{
			UpgradeSpawned = true;
			GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->SpawnUpgrades();
			SpawnCrowdAnim();
			//We want to remove all the balls when the upgrade spawns
			std::vector<GameObject*> Balls = GOMANAGER->FindGameObjectsWithName("ball");
			for (GameObject* ball : Balls)
			{
				ball->GetComponent<CPPSPlayerGolfBall>()->DestroyBall();
			}
			std::vector<GameObject*> Projectiles = GOMANAGER->FindGameObjectsWithName("Projectile");
			for (GameObject* projectile : Projectiles)
			{
				projectile->GetComponent<CPPSBaseGolfBall>()->canDestroy = true;
			}
		}
		//Timer for the crowd, if the crowd texture is active then we want to do stuff
		if (crowdTexture->IsActive())
		{
			timer += TIME->GetDeltaTime();
			Vec2<float> crowdPos{ cachedCrowdPos };
			//temporary thing until we get the anim in
			//This basically just lerps the thing down in 7 seconds
			float smoothTime = timer / crowdTimer;
			//thank you Prof Ronald and Desmos 
			smoothTime = smoothTime * smoothTime * (3.f - 2.f * smoothTime);
			crowdTexture->GetComponent<CPTransform>()->SetPosition(Lerp(crowdPos, Vec2<float>(cachedCrowdPos.x, 0), smoothTime));
			//This one below needs to stay though
			if (timer >= crowdTimer)	//once the sound finishes playing we hide it all and reset pos
			{
				crowdTexture->SetActive(false);
				crowdTexture->GetComponent<CPTransform>()->SetPosition(cachedCrowdPos);
				timer = 0;
			}
		}
	}
	void CPPSGameManager::Destroy()
	{
		//Should be empty
	}

	/*!************************************************************************
	* \brief Function to spawn a random enemy
	* 
	**************************************************************************/
	void CPPSGameManager::SpawnRandomEnemy()
	{
		//First we get a random number generator
		std::random_device rngesus;
		//Then we get a random index from the vector list from 0 to maxsize-1
		std::uniform_int_distribution<int> distribution(0, static_cast<int>(EnemyList.size()) - 1);
		int enemyIndex = distribution(rngesus);	//then we generate the number
		//now we check if we can afford to spawn the enemy
		DebuggerLogFormat("Index : %d, Credits : %d, Cost : %d",
			enemyIndex, SpawnCredits,EnemyList[enemyIndex].second);
		if (SpawnCredits - EnemyList[enemyIndex].second >= 0)
		{
			//then we deduct the cost and spawn the enemy
			SpawnCredits -= EnemyList[enemyIndex].second;
			//EnemyList[enemyIndex].first; //should call the function
			(this->*EnemyList[enemyIndex].first)();
			//Most important god damn line of code
			currentEnemyCount++;
		}
		
	}
	/*!************************************************************************
	* \brief Spawns a mage enemy
	* 
	**************************************************************************/
	void CPPSGameManager::SpawnMageEnemy()
	{
		GameObject* mageClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Mage", *mageClone);
		//mageClone->GetComponent<CPTransform>()->SetPosition(mouse_pos);
	}

	/*!************************************************************************
	* \brief Spawns a chaser enemy
	* 
	**************************************************************************/
	void CPPSGameManager::SpawnChaserEnemy()
	{
		GameObject* chaserClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("EnemyChaser1", *chaserClone);
		//chaserClone->GetComponent<CPTransform>()->SetPosition(mouse_pos);
	}

	/*!************************************************************************
	* \brief Spawns a chaser enemy
	*
	**************************************************************************/
	void CPPSGameManager::SpawnChargerEnemy()
	{
		GameObject* chargerClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Charger", *chargerClone);
		//mageClone->GetComponent<CPTransform>()->SetPosition(mouse_pos);
	}

	void CPPSGameManager::SpawnCrowdAnim()
	{
		//First we play the sound
		AUDIOMANAGER->PlaySound("Spliced_Cheering");
		//then we show the crowd texture
		crowdTexture->SetActive(true);
		//for now the animation will be hard coded
		//but in the future it should be an anim with the same duration as the sound
		

	}

	/*!************************************************************************
	* \brief Function to reduce the enemy count (should be called by base enemy's hurt)
	* 
	**************************************************************************/
	void CPPSGameManager::ReduceEnemyCount()
	{
		DebuggerLogFormat("Enemy count : %d", currentEnemyCount);
		currentEnemyCount--;
		if (currentEnemyCount < 0)
		{
			//By right we should never have this
			DebuggerLogWarning("Enemy Count Error! Please check enemy count logic");
		}
	}
	/*!************************************************************************
	* \brief Function to generate the wave
	* 
	**************************************************************************/
	void CPPSGameManager::GenerateWave()
	{
		//We only want the credits to be affected AFTER the first level
		if (currentWave > 1)
		{	//Formula => 12/(0.5f + e^(-x*10/5 + 4))
			
			SpawnCredits = static_cast<int>(12 / (0.5f + expf(static_cast<float>(-((currentWave * 10) / 5) + 4))));
		}

		//Center of the map is 969 x 540 (game manager's position)
		do {
			SpawnRandomEnemy();
		} while ((SpawnCredits-2) >= 0); //As long as it is more than the current lowest spawn amount
		

	}
	/*!************************************************************************
	* \brief Function to spawn in the next wave
	* 
	**************************************************************************/
	void CPPSGameManager::NextWave()
	{
		currentWave++;
		GenerateWave();
		UpgradeSpawned = false;
	}
}