
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
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Scene/SceneManager.h"
#include "CPPSProjectileBall.h"
#include "Litterbox/Components/RenderVideoComponent.h"
#include "CPPSDirectionHelper.h"
#include "CPPSPlayer.h"
#include "CPPSMouseUI.h"
#include "CPPAudioManager.h"
#include "CPPSBaseEnemy.h"
namespace LB
{
	void CPPSGameManager::Start()
	{
		// Initialising player values
		m_PlayerMaxHealth = 3;
		m_PlayerCurrentHealth = 3;
		m_PlayerMaxBalls = 3;
		cachedMaxBalls = m_PlayerMaxBalls;
		m_PlayerCurrentBalls = 0;
		m_PlayerWalkSpeed = 600.0f;
		m_PlayerMaxSpeed = 800.0f;
		m_PlayerArbitraryFriction = 0.95f;

		//We also need to grab the crowdTexture
		//By default, the render is set active false
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		mAudioManager = GOMANAGER->FindGameObjectWithName("AudioManager");
		crowdTexture = GOMANAGER->FindGameObjectWithName("EndCrowdCheer");
		gameOverTexture = GOMANAGER->FindGameObjectWithName("ActualTexture");
		killerTexture = GOMANAGER->FindGameObjectWithName("Killer");
	
		//Set the player's spawn point
		playerSpawnPoint = GOMANAGER->FindGameObjectWithName("Player Spawn")->GetComponent<CPTransform>()->GetPosition();

		//Damn scuffed way of doing this but we're adding the function ptr and cost to spawn
		//into a list
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnChaserEnemy, 2));
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnMageEnemy, 5));
		EnemyList.emplace_back(std::make_pair(&CPPSGameManager::SpawnChargerEnemy, 8));

		//We grab all the GO's called spawnpoints
		std::vector<GameObject*> temp = GOMANAGER->FindGameObjectsWithName("Spawnpoint");
		for (const auto& go : temp)
		{	//then we add their positions to the vector 
			SpawnPoints.push_back(go->GetComponent<CPTransform>()->GetPosition());
		}
		//Forgive me lord for I have sinned
		if (SCENEMANAGER->GetCurrentScene()->GetName() == "SceneMain")
		{
			currentWave = 1;
		}
		//For the first level we just make it such that it's always 2 melee enemies
		if (currentWave == 1) 
		{
			m_portalHolder = GOMANAGER->FindGameObjectWithName("PortalPosition")->GetComponent<CPTransform>();
			//Extremely bad way of doing this
			ItemLost1 = GOMANAGER->FindGameObjectWithName("ItemLost1")->GetComponent<CPRender>();
			ItemLost2 = GOMANAGER->FindGameObjectWithName("ItemLost2")->GetComponent<CPRender>();
			ItemLost3 = GOMANAGER->FindGameObjectWithName("ItemLost3")->GetComponent<CPRender>();
			ItemLost4 = GOMANAGER->FindGameObjectWithName("ItemLost4")->GetComponent<CPRender>();

			cachedRestartPos = GOMANAGER->FindGameObjectWithName("RestartGameButtonUI")->GetComponent<CPTransform>()->GetLocalPosition();
			cachedQuitPos = GOMANAGER->FindGameObjectWithName("MainMenuButtonUI")->GetComponent<CPTransform>()->GetLocalPosition();

			SpawnCredits = 4;
			GenerateWave();
			GameStart = true;

			// Load maps
			m_mapHolder = GOMANAGER->FindGameObjectWithName("MapHolder")->GetComponent<CPTransform>();
			m_mapList.emplace_back(Vec2<float>{ -670.f, 2566.f }, Vec2<float>{ -540.f, 1620.f }, "Map_TheClassic");
			m_mapList.emplace_back(Vec2<float>{ -1000.f, 2600.f }, Vec2<float>{ -1200.f, 2600.f }, "Map_PinBall");
			m_mapList.emplace_back(Vec2<float>{ -860.f, 3200.f }, Vec2<float>{ -570.f, 1580.f }, "Map_Finals");


			// Classic map is first
			m_currentMap = m_mapList[0];
		}

		// For the tutorial stage
		if (currentWave == 0) 
		{
			FillSpawnPoints("Dummy");
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
		////Manually Spawn Charger Enemy
		if (INPUT->IsKeyTriggered(KeyCode::KEY_P))
		{
			SpawnChargerEnemy();
		}
		if (INPUT->IsKeyTriggered(KeyCode::KEY_G))
		{
			mPlayer->GetComponent<CPPSPlayer>()->isGod = !mPlayer->GetComponent<CPPSPlayer>()->isGod;
			GOMANAGER->FindGameObjectWithName("GodModeIndicator")->SetActive(mPlayer->GetComponent<CPPSPlayer>()->isGod);
			//GenerateWave();
			//GameStart = true;
		}
		if (INPUT->IsKeyTriggered(KeyCode::KEY_U))
		{
			//GOMANAGER->FindGameObjectWithName("GameMusic")->GetComponent<CPAudioSource>()->FadeOut(5.f);
			VideoPlayerSystem::Instance()->PlayCutscene("samplevideo", "SceneMainMenu");
			//ShowGameWin();
		}
		//Infinite Ammo
		if (INPUT->IsKeyTriggered(KeyCode::KEY_7))
		{	//If we're infinite ammo
			if (!isInfiniteAmmo)
			{
				cachedMaxBalls = m_PlayerMaxBalls;
				isInfiniteAmmo = true;
			}
			else
			{
				//DestroyAllBalls();
				m_PlayerMaxBalls = cachedMaxBalls;
				m_PlayerCurrentBalls = 0;
				isInfiniteAmmo = false;
			}
		}
		//Gain Health
		if (INPUT->IsKeyTriggered(KeyCode::KEY_8))
		{
			if(m_PlayerCurrentHealth < m_PlayerMaxHealth)
			m_PlayerCurrentHealth++;
		}
		//Lose Health
		if (INPUT->IsKeyTriggered(KeyCode::KEY_9))
		{
			if (m_PlayerCurrentHealth > 1)
			{
				m_PlayerCurrentHealth--;
			}
			else
			{
				m_PlayerCurrentHealth--;
				ShowGameOver(*GameObj);
			}
		}
		/*if (INPUT->IsKeyTriggered(KeyCode::KEY_I))
		{
			GOMANAGER->FindGameObjectWithName("GameMusic")->GetComponent<CPAudioSource>()->FadeIn(5.f,0.5f);
		}*/
		//if (INPUT->IsKeyTriggered(KeyCode::KEY_W))
		//{
		//	VideoPlayerSystem::Instance()->PlayCutscene("SplashScreenDigiPen", "SceneMainMenu");
		//}
		//Test function to see if the remove gameobject code works
		//You have to comment out the ball's canDestroy code in order for this
		//to not crash the game
		//if (INPUT->IsKeyTriggered(KeyCode::KEY_V))
		//{
		//	GameObject* ballObject = FACTORY->SpawnGameObject();
		//	JSONSerializer::DeserializeFromFile("ball", *ballObject);
		//	GOMANAGER->RemoveGameObject(ballObject, 2.f);
		//}

		
		////Really really really scuffed way of doing this
		//if (currentEnemyCount == 0 && GameStart && UpgradePicked)
		//{
		//	//Spawn Upgrades, Do level transition blablabla
		//	currentWave++;
		//	GenerateWave();
		//	UpgradePicked = false;
		//}

		// For tutorial upgrade creation
		if (currentWave == 0 && SpawnedeEnemiesList.empty() && GameStart && !UpgradeSpawned)
		{
			UpgradeSpawned = true;
			GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->SpawnUpgrades();
			DestroyAllBalls();
			
		}
		
		//If game's started, upgrade hasn't spawned and no enemies and not tutorial
		if (SpawnedeEnemiesList.empty() && GameStart && !UpgradeSpawned && currentWave &&!isGameOver)
		{
			UpgradeSpawned = true;
			if (currentWave % 2)
			{
				GOMANAGER->FindGameObjectWithName("Portal")->SetActive(true);
			}
			else
			{
				GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->SpawnUpgrades();

			}
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
				projectile->GetComponent<CPPSProjectileBall>()->canDestroy = true;
			}
		}
		//Timer for the crowd, if the crowd texture is active then we want to do stuff
		if (crowdTexture && crowdTexture->IsActive())
		{
			timer += static_cast<float>(TIME->GetDeltaTime());

			/*
			Vec2<float> crowdPos{ cachedCrowdPos };
			//temporary thing until we get the anim in
			//This basically just lerps the thing down in 7 seconds
			float smoothTime = timer / crowdTimer;
			//thank you Prof Ronald and Desmos 
			smoothTime = smoothTime * smoothTime * (3.f - 2.f * smoothTime);
			crowdTexture->GetComponent<CPTransform>()->SetPosition(Lerp(crowdPos, Vec2<float>(cachedCrowdPos.x, 0), smoothTime));
			*/

			//This one below needs to stay though
			if (timer >= crowdTimer)	//once the sound finishes playing we hide it all and reset pos
			{
				crowdTexture->SetActive(false);
				//crowdTexture->GetComponent<CPTransform>()->SetPosition(cachedCrowdPos);
				timer = 0;
			}
		}

		//Update the GAME OVER UI 
		if (gameOverTexture->IsActive())
		{
			//We get the mouse position
			mouse_pos = GOMANAGER->FindGameObjectWithName("MouseUI")->GetComponent<CPPSMouseUI>()->GetComponent<CPTransform>()->GetPosition();
			//Then we get all the colliders near the mouse
			bool _restartHovered{ false }, _quitHovered{ false };
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);
			for (const auto& col : vec_colliders) //then we loop through all the cols to find our buttons
			{
				if (col->gameObj->GetName() == "RestartGameButton")
				{
					if (!restartHovered)
					{
						GOMANAGER->FindGameObjectWithName("RestartGameButtonUI")->GetComponent<CPTransform>()->SetPosition(cachedRestartPos + Vec2<float>(0, 40));
						restartHovered = true;
					}
					_restartHovered = true;
					break;
				}
				if (col->gameObj->GetName() == "MainMenuButton")
				{
					if (!quitHovered)
					{
						GOMANAGER->FindGameObjectWithName("MainMenuButtonUI")->GetComponent<CPTransform>()->SetPosition(cachedQuitPos + Vec2<float>(0, 40));
						quitHovered = true;
					}
					_quitHovered = true;
					break;
				}
			}
			if (restartHovered && !_restartHovered)
			{
				GOMANAGER->FindGameObjectWithName("RestartGameButtonUI")->GetComponent<CPTransform>()->SetPosition(cachedRestartPos);
				restartHovered = false;
			}
			if (quitHovered && !_quitHovered)
			{
				GOMANAGER->FindGameObjectWithName("MainMenuButtonUI")->GetComponent<CPTransform>()->SetPosition(cachedQuitPos);
				quitHovered = false;
			}

			if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1)) //if we click we see what we clicked on
			{
				for (const auto& col : vec_colliders) //then we loop through all the cols to find our buttons
				{
					if (col->gameObj->GetName() == "RestartGameButton")
					{
						gameOverTexture->SetActive(false);
						//std::cout << "Restart Game!\n";
						SCENEMANAGER->ReloadScene();
						break;
					}
					if (col->gameObj->GetName() == "MainMenuButton")
					{
						//std::cout << "GotoMainMenu!\n";
						gameOverTexture->SetActive(false);
						SCENEMANAGER->LoadScene("SceneMainMenu");
						break;
					}
				}
			}
			//if (!isSoundSwapped && !GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPAudioSource>()->volume)
			//{
			//	//more sinful code :pensive:
			//	GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPAudioSource>()->UpdateAudio("GameOverBGM");
			//	GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPAudioSource>()->FadeIn(2.f,0.4f);
			//	isSoundSwapped = true;
			//}
		}
	}
	void CPPSGameManager::Destroy()
	{
		//Should be empty
	}

	void CPPSGameManager::DeleteOldMap()
	{
		// Clear the old map
		if (m_mapHolder)
		{
			GOMANAGER->RemoveGameObject(m_mapHolder->GetChild(0)->gameObj);
			m_mapHolder->RemoveChild(0);
		}
	}

	void CPPSGameManager::StartNewMap()
	{
		// Load the new map
		GameObject* newMap = FACTORY->SpawnGameObject();
		m_mapHolder->AddChild(newMap->GetComponent<CPTransform>());

		m_currentMap = m_mapList[rand() % m_mapList.size()];
		JSONSerializer::DeserializeFromFile(m_currentMap.m_name, *newMap);

		// Setup spawnpoints
		playerSpawnPoint = GOMANAGER->FindGameObjectWithName("Player Spawn")->GetComponent<CPTransform>()->GetPosition();

		SpawnPoints.clear();
		std::vector<GameObject*> temp = GOMANAGER->FindGameObjectsWithName("Spawnpoint");
		for (const auto& go : temp)
		{	//then we add their positions to the vector 
			SpawnPoints.push_back(go->GetComponent<CPTransform>()->GetPosition());
		}

		// Move portal
		m_portalHolder = GOMANAGER->FindGameObjectWithName("PortalPosition")->GetComponent<CPTransform>();
		GOMANAGER->FindGameObjectWithName("Portal")->GetComponent<CPTransform>()->SetPosition(m_portalHolder->GetPosition());

		GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->UpgradePos = GOMANAGER->FindGameObjectWithName("UpgradePosition")->GetComponent<CPTransform>()->GetPosition();
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
			//should call the function
			//god damn this syntax is so cursed
			(this->*EnemyList[enemyIndex].first)();
			//Most important god damn line of code [edit: no longer the most important line of code]
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
		mageClone->GetComponent<CPTransform>()->SetPosition(GetRandomSpawnPoint());
		SpawnedeEnemiesList.push_back(mageClone);
	}

	/*!************************************************************************
	* \brief Spawns a chaser enemy
	* 
	**************************************************************************/
	void CPPSGameManager::SpawnChaserEnemy()
	{
		GameObject* chaserClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("EnemyChaser1", *chaserClone);
		chaserClone->GetComponent<CPTransform>()->SetPosition(GetRandomSpawnPoint());
		SpawnedeEnemiesList.push_back(chaserClone);
	}

	/*!************************************************************************
	* \brief Spawns a chaser enemy
	*
	**************************************************************************/
	void CPPSGameManager::SpawnChargerEnemy()
	{
		GameObject* chargerClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Charger_Shield", *chargerClone);
		//JSONSerializer::DeserializeFromFile("Charger", *chargerClone);
		chargerClone->GetComponent<CPTransform>()->SetPosition(GetRandomSpawnPoint());
		SpawnedeEnemiesList.push_back(chargerClone);
	}

	/*!************************************************************************
	 * \brief Spawns the crowd anim
	 * 
	**************************************************************************/
	void CPPSGameManager::SpawnCrowdAnim()
	{	//If the player is dead, we kinda don't want to spawn the crowd
		//if (isGameOver) return;
		//First we play the sound
		mAudioManager->GetComponent<CPPSAudioManager>()->Play2DSound("Spliced_Cheering",false , 0.3f);
		//then we show the crowd texture
		crowdTexture->SetActive(true);
		crowdTexture->GetComponent<CPTransform>()->GetChild(0)->GetComponent<CPAnimator>()->Play("Action_UpDownCrowd");
		crowdTexture->GetComponent<CPTransform>()->GetChild(1)->GetComponent<CPAnimator>()->Play("Action_UpDownCrowd");
		//for now the animation will be hard coded
		//but in the future it should be an anim with the same duration as the sound
		

	}
	
	/*!************************************************************************
	 * \brief Spawns a dummy enemy (used in tutorial)
	 * 
	**************************************************************************/
	void CPPSGameManager::SpawnDummyEnemy()
	{
		GameObject* dummyClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Dummy", *dummyClone);
		dummyClone->GetComponent<CPTransform>()->SetPosition(GetRandomSpawnPoint());
		SpawnedeEnemiesList.push_back(dummyClone);
		// Need to increment it here as we are not adding it to the list of enemies
		currentEnemyCount++;
	}
	
	/*!************************************************************************
	* \brief Function to reduce the enemy count (should be called by base enemy's hurt)
	* (DEPRACTD DO NOT USE)
	**************************************************************************/
	void CPPSGameManager::ReduceEnemyCount()
	{
		DebuggerLogFormat("Enemy count : %d", currentEnemyCount);
		--currentEnemyCount;

		//if (currentEnemyCount == 1)
		//{
		//	GOMANAGER->FindGameObjectWithName("DirectionHelper")->GetComponent<CPPSDirectionHelper>()->mLastEnemy = true;
		//}
		//else if (currentEnemyCount == 0)
		//{
		//	GOMANAGER->FindGameObjectWithName("DirectionHelper")->GetComponent<CPPSDirectionHelper>()->mLastEnemy = false;
		//}

		//if (currentEnemyCount < 0)
		//{
		//	//By right we should never have this
		//	DebuggerLogWarning("Enemy Count Error! Please check enemy count logic");
		//}
	}

	/*!************************************************************************
	 * \brief Removes enemy from the list of enemies
	 * 
	 * \param enemyToRemove Enemy to remove
	**************************************************************************/
	void CPPSGameManager::RemoveSpawnedEnemy(GameObject* enemyToRemove)
	{
		DebuggerLogFormat("Spawned Enemy Count : %d", SpawnedeEnemiesList.size());
		onEnemyKill.Invoke();
		//If the player kills an enemy (that is NOT the last enemy), we play a positive sound
		if (SpawnedeEnemiesList.size() > 1)
		{
			mAudioManager->GetComponent<CPPSAudioManager>()->Play2DSound(mAudioManager->GetComponent<CPPSAudioManager>()->CrowdPositiveSounds, false, 0.2f);
		}
		auto itr = std::find(SpawnedeEnemiesList.begin(), SpawnedeEnemiesList.end(), enemyToRemove);
		if (itr != SpawnedeEnemiesList.end())
		{
			SpawnedeEnemiesList.erase(itr);
		}
	}
	/*!************************************************************************
	 * \brief Function to show gameover screen
	 * 
	 * \param enemyObj Enemy that killed the player
	**************************************************************************/
	void CPPSGameManager::ShowGameOver(GameObject enemyObj)
	{
		//first we fade out the music	
		//GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPAudioSource>()->FadeOut(2.5f);
		//Cross fade it out babeyyy still WIP though
		mAudioManager->GetComponent<CPPSAudioManager>()->CrossFadeBGM("GameOverBGM", 2.5f);
		mAudioManager->GetComponent<CPPSAudioManager>()->Play2DSound("GameOver", false, 0.5f);

		isGameOver = true;
		//We kill all enemies
		for (GameObject* enemy : SpawnedeEnemiesList)
		{
			enemy->SetActive(false);
		}
		SpawnedeEnemiesList.clear();

		killerTexture->SetActive(true);
		//We see who the killer is 
		//0 = chaser , 1 = mage, 2 = charger, 3 = bramble
		if (enemyObj.GetName() == "EnemyChaser1")
		{
			//std::cout << "Killed by chaser\n";
			//Default is chaser so we don't do anything
		}
		else if (enemyObj.GetName() == "Projectile" || enemyObj.GetName() == "Mage")
		{
			//std::cout << "Killed by a mage\n";
			killerTexture->GetComponent<CPRender>()->SetSpriteTexture(killerTexture->GetComponent<CPRender>()->spriteSheetName, 1);
		}
		else if (enemyObj.GetName() == "Charger_Shield" || enemyObj.GetName() == "Shield")
		{
			//std::cout << "Killed by charger\n";
			killerTexture->GetComponent<CPRender>()->SetSpriteTexture(killerTexture->GetComponent<CPRender>()->spriteSheetName, 2);

		}
		else if (enemyObj.GetName() == "Bramble")
		{
			//std::cout << "Killed by carelessness\n";
			killerTexture->GetComponent<CPRender>()->SetSpriteTexture(killerTexture->GetComponent<CPRender>()->spriteSheetName, 3);
		}
		else 
		{
			//std::cout << "Killed by " << enemyObj.GetName() << '\n';
		} 
		switch (GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->upgradeCount)
		{
		case 1:
			ItemLost1->gameObj->SetActive(true);
			break;
		case 2:
			ItemLost1->gameObj->SetActive(true);
			ItemLost2->gameObj->SetActive(true);
			break; 
		case 3:
			ItemLost1->gameObj->SetActive(true);
			ItemLost2->gameObj->SetActive(true);
			ItemLost3->gameObj->SetActive(true);
			break;
		case 4:
			ItemLost1->gameObj->SetActive(true);
			ItemLost2->gameObj->SetActive(true);
			ItemLost3->gameObj->SetActive(true);
			ItemLost4->gameObj->SetActive(true);
			break;
		}
		// Show UI
		gameOverTexture->SetActive(true);
		GOMANAGER->FindGameObjectWithName("RestartGameButtonUI")->SetActive(true);
		GOMANAGER->FindGameObjectWithName("MainMenuButtonUI")->SetActive(true);
	}

	/*!************************************************************************
	 * \brief Function to send player to the winning cutscene
	 * 
	**************************************************************************/
	void CPPSGameManager::ShowGameWin()
	{
		VideoPlayerSystem::Instance()->PlayCutscene("samplevideo", "SceneMainMenu");
	}

	/*!************************************************************************
	 * \brief Function to send player to the winning cutscene
	 *
	**************************************************************************/
	void CPPSGameManager::ShowSplashScreen()
	{
		VideoPlayerSystem::Instance()->PlayCutscene("SplashScreen", "SceneMainMenu");
	}

	/*!************************************************************************
	 * \brief Function to get a random spawn point for the enemies
	 * 
	 * \return Vec2<float> random spawn point 
	**************************************************************************/
	Vec2<float> CPPSGameManager::GetRandomSpawnPoint()
	{
		//First we get a random number generator
		std::random_device rngesus;
		//Then we get a random index from the vector list from 0 to maxsize-1
		std::uniform_int_distribution<int> distribution(0, static_cast<int>(SpawnPoints.size()) - 1);
		int spawnIndex = distribution(rngesus);	//then we generate the number
		if (!SpawnPoints.empty())
		{
			return SpawnPoints[spawnIndex];
		}
		DebuggerLogWarning("Somehow unable to find a valid spawnpoint for enemy!");
		return Vec2<float>();
	}

	void CPPSGameManager::FillSpawnPoints(std::string name) 
	{
		for (Vec2<float> pos : SpawnPoints) 
		{
			GameObject* dummyClone = FACTORY->SpawnGameObject();
			JSONSerializer::DeserializeFromFile(name, *dummyClone);
			dummyClone->GetComponent<CPTransform>()->SetPosition(pos);
			// Need to increment it here as we are not adding it to the list of enemies
			SpawnedeEnemiesList.push_back(dummyClone);
		}
	}
	
	/*!************************************************************************
	 * \brief Gets the current wave
	 * 
	 * \return int current wave
	**************************************************************************/
	int CPPSGameManager::GetCurrentWave()
	{
		return currentWave;
	}

	/*!************************************************************************
	 * \brief Gets the current number of enemies
	 *
	**************************************************************************/
	int CPPSGameManager::GetCurrentEnemyCount() const
	{
		return currentEnemyCount;
	}

	void CPPSGameManager::DestroyAllBalls()
	{
		std::vector<GameObject*> Balls = GOMANAGER->FindGameObjectsWithName("ball");
		for (GameObject* ball : Balls)
		{
			ball->GetComponent<CPPSPlayerGolfBall>()->DestroyBall();
		}
	}

	/*!************************************************************************
	 * \brief Gets the SpawnedeEnemiesList member variable
	 *
	**************************************************************************/
	std::vector<GameObject*>& CPPSGameManager::GetSpawnedEnemyList()
	{
		return SpawnedeEnemiesList;
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
		if (currentWave == 1)
		{
			SpawnCredits = 4;
		}
		GenerateWave();
		UpgradeSpawned = false;
		GOMANAGER->FindGameObjectWithName("Upgrade Manager")->GetComponent<CPPSUpgradeManager>()->SetSpawned(false);

		isMovementDisabled = false;	//we allow the player to move again
		//we need to place the player back a the correct place and reset their pos and everything
		/*mPlayer->GetComponent<CPTransform>()->SetPosition(playerSpawnPoint);
		mPlayer->GetComponent<CPTransform>()->SetScale(Vec2<float>(1, 1));
		mPlayer->GetComponent<CPTransform>()->SetRotation(0);*/
		//By right should set the camera to player too
	}
	
	/*!************************************************************************
	 * \brief Global function for events to show lose game screen
	 * 
	 * \param enemyObj Enemy that killed the player 
	**************************************************************************/
	void ShowGameOver(GameObject enemyObj)
	{
		//enemy obj is the one that killed the player
		GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->ShowGameOver(enemyObj);
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->PlayerDeathSounds, 0.25f);
		GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>()->Play2DSound("PlayerDeath",false, 0.25f);
		GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>()->Play2DSound("Crowd Gasp_Dead", false, 0.25f);

	}

}