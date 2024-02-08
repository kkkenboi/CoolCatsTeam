#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include <vector>
#include <random>
namespace LB
{
	enum UpgradeType
	{
		SPLIT = 1,		//Ball splits into 3 (not counted towards total ball count)
		BOMB,	//Ball explodes on contact
		BIGBALL,	//Ball increase size and mass
		MOVESPEED,	//Increase player movespeed
		MOREHEALTH,	//Increase Max health by 1 (capped at 6/9)
		MOREBALL,	//Increase ball count by 3	(capped at 6/9)
		MAXCOUNT	//Enum to keep track of the count of upgrades
	};

	class CPPSUpgradeManager : public CPPBehaviour , public Singleton<CPPSUpgradeManager>
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		void SpawnUpgrades(); //Function to spawn 3 upgrades from list of upgrades
		void RemoveUpgradeFromList(UpgradeType upgradeType);
		void HideUpgrades(int chosen);
		void SetBallUpgrade(int upgradeType);
		int GetBallUpgrades();

		//Really scuffed way of making upgrades
		//Upgrade manager assigns the upgrade type to the upgrade
		//then the upgrade tells the upgrade manager which upgrades
		//have been picked, and the upgrade manager tells the 
		//ball what upgrades it has when it spawns
		//note: maybe use a stack instead, randomise the values before putting in
		//then use push and pop 
		std::vector<UpgradeType> UpgradesList;
		//Default is 0 for no upgrades
		int currentBallUpgrades{0};
		bool hasUpgraded{ false };
	private:
		//Temporary upgrade positions for now, in the future these will be calculated 
		//and placed at the "end point" of the bigger map
		std::vector<Vec2<float>> UpgradePositions{ {650.f,785.f}, {950.f,785.f} ,{1250.f,785.f} };
		GameObject* leftUpgrade{ nullptr };
		GameObject* middleUpgrade{ nullptr };
		GameObject* rightUpgrade{ nullptr };
		bool isSpawned{ false };
	};
	REGISTER_SCRIPT(CPPSUpgradeManager)

}