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
		void Start() override;
		void Update() override;
		void Destroy() override;

		void GenerateWave();

		//Need an array of game objects which will be the enemies to spawn
		//Assign values to each enemy
		//Each wave also has a certain value
		//Spawn enemy and deduct the values
		GameObject* mPlayer;
		//Pairs of enemy to value cost for the game manager to spawn
		std::vector<std::pair<GameObject*, float>> EnemyPrefabList;

	private:
		//Formula made in desmos, curve is a sexy sexy S curve.
		//Formula => 12/(0.5f + e^(-x/5 + 4))
		int SpawnCredits{}; //Spawn credits allowed for the game manager 
		int currentWave{ 1 };	//starting wave is always 1
		//We try to store references to the enemies
		GameObject* mageEnemy;
		GameObject* chaserEnemy;
	};
}