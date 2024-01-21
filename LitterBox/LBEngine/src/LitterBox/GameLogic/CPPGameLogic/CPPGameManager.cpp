

#include "CPPGameManager.h"
namespace LB
{
	void CPPSGameManager::Start()
	{
		//loading the gameobjects with data
		JSONSerializer::DeserializeFromFile("Mage", *mageEnemy);
		JSONSerializer::DeserializeFromFile("EnemyChaser1", *chaserEnemy);
		//then adding it to our pair list
		EnemyPrefabList.emplace_back(std::make_pair(chaserEnemy, 2));
		EnemyPrefabList.emplace_back(std::make_pair(mageEnemy, 5));
		//For the first level we just make it such that it's always 2 melee enemies
		if (currentWave == 1) 
		{
			SpawnCredits = 4;
		}
	}
	void CPPSGameManager::Update()
	{
		//We only want the credits to be affected AFTER the first level
		if (currentWave > 1)
		{	//Formula => 12/(0.5f + e^(-x/5 + 4))
			SpawnCredits = 12 / (0.5f + expf(-(currentWave / 5) + 4));
		}


	}
	void CPPSGameManager::Destroy()
	{
		//Should be empty
	}
}