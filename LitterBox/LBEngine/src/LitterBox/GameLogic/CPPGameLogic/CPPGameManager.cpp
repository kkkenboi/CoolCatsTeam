
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
		m_PlayerWalkSpeed = 1750.0f;
		m_PlayerMaxSpeed = 3500.0f;
		m_PlayerArbitraryFriction = 0.95f;


		//loading the gameobjects with data
		//then adding it to our pair list
		//EnemyPrefabList.emplace_back(std::make_pair(chaserEnemy, 2));
		//EnemyPrefabList.emplace_back(std::make_pair(mageEnemy, 5));

		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnChaserEnemy, 2));
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnMageEnemy, 5));
		//in the future
		//EnemyList.emplace_back(std::make_pair(SpawnChargerEnemy, 8));

		//In the future, Charger enemy will be 8 credits probably
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