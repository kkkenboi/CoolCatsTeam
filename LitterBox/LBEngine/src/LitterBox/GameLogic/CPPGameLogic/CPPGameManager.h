/*!************************************************************************
 * \file				CPPGameManager.h
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2451A
 * \date				09/02/2024
 * \brief 				This header file contains the declarations for the 
 * 						game manager that handles the AI director of the game
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Serialization/AssetManager.h"
#include <vector>
#include <utility> //for std::pair
namespace LB
{
	class CPPSGameManager : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief 
		* Component functions for the Game Manager
		**************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		/*!************************************************************************
		 * \brief Function to generate the wave
		 * 
		**************************************************************************/
		void GenerateWave();

		/*!************************************************************************
		 * \brief Function to spawn in the next wave
		 * 
		**************************************************************************/
		void NextWave();

		/*!************************************************************************
		 * \brief Function to spawn a random enemy
		 * 
		**************************************************************************/
		void SpawnRandomEnemy();

		/*!************************************************************************
		 * \brief Spawns a mage enemy
		 * 
		**************************************************************************/
		void SpawnMageEnemy();

		/*!************************************************************************
		 * \brief Spawns a chaser enemy
		 * 
		**************************************************************************/
		void SpawnChaserEnemy();

		/*!************************************************************************
		* \brief Spawns a chaser enemy
		*
		**************************************************************************/
		void SpawnChargerEnemy();

		void SpawnCrowdAnim();

		/*!************************************************************************
		 * \brief Function to reduce the enemy count (should be called by base enemy's hurt)
		 * 
		**************************************************************************/
		void ReduceEnemyCount();

		void ShowGameOver(GameObject enemyObj);
		//Need an array of game objects which will be the enemies to spawn
		//Assign values to each enemy
		//Each wave also has a certain value
		//Spawn enemy and deduct the values
		GameObject* mPlayer{nullptr};
		//Pairs of enemy to value cost for the game manager to spawn
		//std::vector<std::pair<GameObject*, float>> EnemyPrefabList;
		std::vector < std::pair<void(CPPSGameManager::*)(), int>> EnemyList;
		bool GameStart{ false };

		int m_PlayerMaxHealth{};
		int m_PlayerCurrentHealth{};
		int m_PlayerMaxBalls{};
		int m_PlayerCurrentBalls{};
		float m_PlayerWalkSpeed{};
		float m_PlayerMaxSpeed{};
		float m_PlayerArbitraryFriction{};
		
		//Cheering sound is 7 seconds
		float crowdTimer{7.f};
		float timer{};	//arbitrary timer for proper lerping
		bool isGameOver{ false };
	private:
		bool UpgradeSpawned{ false };
		//Formula made in desmos, curve is a sexy sexy S curve.
		//Formula => 12/(0.5f + e^(-x/5 + 4))
		int SpawnCredits{}; //Spawn credits allowed for the game manager 
		int currentWave{ 1 };	//starting wave is always 1
		int currentEnemyCount{ 0 }; //Really scuffed way of tracking enemies imo
		//We try to store references to the enemies
		//GameObject* mageEnemy{nullptr};
		//GameObject* chaserEnemy{ nullptr };
		//GameObject* chargerEnemy{ nullptr };
		GameObject* crowdTexture{ nullptr };
		GameObject* gameOverTexture{ nullptr };
		Vec2<float> cachedCrowdPos{};
		Vec2<float> mouse_pos{};

	};
	void ShowGameOver(GameObject enemyObj);
	REGISTER_SCRIPT(CPPSGameManager)
}