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
	// Details of a map, to be refactored
	struct MapDetails
	{
		MapDetails() {}
		MapDetails(Vec2<float> xbounds, Vec2<float> ybounds, std::string const& name)
			: m_xbounds(xbounds), m_ybounds(ybounds), m_name(name) { }
		Vec2<float> m_xbounds;
		Vec2<float> m_ybounds;
		std::string m_name;
	};

	class CPPSGameManager : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief
		* Start function for the Game Manager, initialises the player values and
		* waves and maps
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief
		* Update function for the Game Manager, checks for input and updates the game
		**************************************************************************/
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

		/*!************************************************************************
		 * \brief Spawns the crowd anim
		 * 
		**************************************************************************/
		void SpawnCrowdAnim();
		
		/*!************************************************************************
		 * \brief Spawns a dummy
		 * 
		**************************************************************************/
		void SpawnDummyEnemy();

		/*!************************************************************************
		 * \brief Function to reduce the enemy count (should be called by base enemy's hurt)
		 * (DEPRECATED)
		**************************************************************************/
		void ReduceEnemyCount();
		Event<> onEnemyKill;
		Event<> onNextLevel;

		/*!************************************************************************
		 * \brief Function to remove a spawned enemy from the enemy list
		 * 
		 * \param enemyToRemove Enemy to remove from the list
		**************************************************************************/
		void RemoveSpawnedEnemy(GameObject* enemyToRemove);

		/*!************************************************************************
		 * \brief Function to show the gameover screen
		 * 
		 * \param enemyObj Enemy that killed the player
		**************************************************************************/
		void ShowGameOver(GameObject enemyObj);

		/*!************************************************************************
		 * \brief Function to show the game win (basically transitions to win cutscene)
		 * 
		**************************************************************************/
		void ShowGameWin();

		/*!************************************************************************
		 * \brief Function to show the splashscreen
		 *
		**************************************************************************/
		void ShowSplashScreen();

		/*!************************************************************************
		 * \brief Get the Random Spawn Point object
		 * 
		 * \return Vec2<float> Position of random spawn point object
		**************************************************************************/
		Vec2<float> GetRandomSpawnPoint();

		/*!************************************************************************
		 * \brief Get random spawn points (USED IN TUTORIAL)
		 * 
		 * \param name Name of the spawn point object
		**************************************************************************/
		void FillSpawnPoints(std::string name);

		/*!************************************************************************
		 * \brief Get the Current Wave object
		 * 
		 * \return int current wave
		**************************************************************************/
		int GetCurrentWave();

		/*!************************************************************************
		 * \brief Gets the current number of enemies
		 *
		**************************************************************************/
		int GetCurrentEnemyCount() const;

		void DestroyAllBalls();
		/*!************************************************************************
		 * \brief Gets the SpawnedeEnemiesList member variable
		**************************************************************************/
		std::vector<GameObject*>& GetSpawnedEnemyList();

		static GameObject*& getGameOverTexture() { return gameOverTexture; }

		// Event for new map start
		Event<> onNewMapStart;

		/*!************************************************************************
		 * \brief Clears the gameobject containing the old map
		**************************************************************************/
		void DeleteOldMap();
		/*!************************************************************************
		 * \brief Creates a gameobject containing the new map
		**************************************************************************/
		void StartNewMap();
		int m_lastMapIndex{ -1 }; // The last map index loaded

		//Need an array of game objects which will be the enemies to spawn
		//Assign values to each enemy
		//Each wave also has a certain value
		//Spawn enemy and deduct the values
		GameObject* mPlayer{nullptr};
		GameObject* mAudioManager{ nullptr };
		//Pairs of enemy to value cost for the game manager to spawn
		//std::vector<std::pair<GameObject*, float>> EnemyPrefabList;
		std::vector < std::pair<void(CPPSGameManager::*)(), int>> EnemyList;
		bool GameStart{ false };
		
		//Player stats
		int m_PlayerMaxHealth{};
		int m_PlayerCurrentHealth{};
		int m_PlayerMaxBalls{};
		int m_PlayerCurrentBalls{};
		int m_PlayerMaxUpgrades{ 4 };
		int m_PlayerCurrentUpgrades{};
		float m_PlayerWalkSpeed{};
		float m_PlayerMaxSpeed{};
		float m_PlayerArbitraryFriction{};
		//We cache the player spawnpoint
		Vec2<float> playerSpawnPoint{};
		//Cheering sound is 7 seconds
		float crowdTimer{7.f};
		float timer{};	//arbitrary timer for proper lerping
		bool isGameOver{ false };
		bool isMovementDisabled{ false };
		//forgive me for I have sinned yet again
		CPRender* ItemLost1{ nullptr };
		CPRender* ItemLost2{ nullptr };
		CPRender* ItemLost3{ nullptr };
		CPRender* ItemLost4{ nullptr };

		// Current map loaded
		MapDetails m_currentMap;
		bool isInfiniteAmmo{ false };

	private:
		bool UpgradeSpawned{ false };
		//Formula made in desmos, curve is a sexy sexy S curve.
		//Formula => 12/(0.5f + e^(-x/5 + 4))
		int SpawnCredits{}; //Spawn credits allowed for the game manager 
		int currentWave{ 0 };	//starting wave is now 0 due to tutorial level
		int currentEnemyCount{ 0 }; //Really scuffed way of tracking enemies imo
		//We try to store references to the enemies
		//GameObject* mageEnemy{nullptr};
		//GameObject* chaserEnemy{ nullptr };
		//GameObject* chargerEnemy{ nullptr };
		GameObject* crowdTexture{ nullptr };
		static GameObject* gameOverTexture;
		GameObject* killerTexture{ nullptr }; //texture of the player killer for gameover
		Vec2<float> cachedCrowdPos{}, cachedRestartPos{}, cachedQuitPos{};
		bool restartHovered{ false }, quitHovered{ false };

		// Map stuff
		CPTransform* m_mapHolder{ nullptr };
		CPTransform* m_portalHolder{ nullptr };
		std::vector<MapDetails> m_mapList;

		Vec2<float> mouse_pos{};
		std::vector<Vec2<float>> SpawnPoints;
		bool isSoundSwapped{ false };
		std::vector<GameObject*> SpawnedeEnemiesList;
		int cachedMaxBalls{3};
	};
	void ShowGameOver(GameObject enemyObj);
	REGISTER_SCRIPT(CPPSGameManager)
}