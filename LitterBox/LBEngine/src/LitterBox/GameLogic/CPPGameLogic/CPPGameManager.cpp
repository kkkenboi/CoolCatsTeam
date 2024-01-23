
#include "CPPGameManager.h"
#include "LitterBox/Engine/Input.h"
#include <random>
namespace LB
{
	REGISTER_SCRIPT(CPPSGameManager);
	void CPPSGameManager::Start()
	{
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
			//SpawnChaserEnemy();
			SpawnRandomEnemy();
		}

		////Manually Spawn Mage Enemy
		if (INPUT->IsKeyTriggered(KeyCode::KEY_M))
		{
			SpawnMageEnemy();
		}
	}
	void CPPSGameManager::Destroy()
	{
		//Should be empty
	}

	void CPPSGameManager::SpawnRandomEnemy()
	{
		//First we get a random number generator
		std::random_device rngesus;
		//Then we get a random index from the vector list from 0 to maxsize-1
		std::uniform_int_distribution<int> distribution(0, EnemyList.size() - 1);
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
		}
		
	}

	void CPPSGameManager::SpawnMageEnemy()
	{
		GameObject* mageClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Mage", *mageClone);
		//mageClone->GetComponent<CPTransform>()->SetPosition(mouse_pos);
	}

	void CPPSGameManager::SpawnChaserEnemy()
	{
		GameObject* chaserClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("EnemyChaser1", *chaserClone);
		//chaserClone->GetComponent<CPTransform>()->SetPosition(mouse_pos);
	}

	void CPPSGameManager::GenerateWave()
	{
		//We only want the credits to be affected AFTER the first level
		if (currentWave > 1)
		{	//Formula => 12/(0.5f + e^(-x/5 + 4))
			SpawnCredits = 12 / (0.5f + expf(-(currentWave / 5) + 4));
		}
		//Center of the map is 969 x 540 (game manager's position)
		while (SpawnCredits > 2) //As long as it is more than the current lowest spawn amount
		{
			SpawnRandomEnemy();
		}

	}

}