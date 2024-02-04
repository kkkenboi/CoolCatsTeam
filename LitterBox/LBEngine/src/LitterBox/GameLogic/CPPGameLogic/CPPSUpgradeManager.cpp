#include "CPPSUpgradeManager.h"

namespace LB
{
	void CPPSUpgradeManager::Start()
	{
		//First we populate the upgrade list with all the upgrade types
		for (int i{ 1 }; i < UpgradeType::MAXCOUNT; ++i)
		{
			UpgradesList.push_back(static_cast<UpgradeType>(i));
		}
		//After that we shuffle it up
		//(using default_random_engine will make it "seeded", change in the future!)
		// Current order is : morehealth, bomb, moreball,split,movespeed,bigball
		//514263
		auto rngesus = std::default_random_engine{};
		std::shuffle(UpgradesList.begin(), UpgradesList.end(), rngesus);
		//Debug info
		std::cout << "Upgrade order\n";
		for (UpgradeType upgrade : UpgradesList)
		{
			std::cout << static_cast<int>(upgrade) << '\n';
		}
	}

	void CPPSUpgradeManager::Update()
	{
	}

	void CPPSUpgradeManager::Destroy()
	{
	}

	void CPPSUpgradeManager::SpawnUpgrades()
	{
	}

	void CPPSUpgradeManager::HideUpgrades()
	{
	}

	void CPPSUpgradeManager::SetBallUpgrade(int upgradeType)
	{
		//We just use it as ints for now to set ball upgrades
		currentBallUpgrades |= (1 << upgradeType);
	}

	int CPPSUpgradeManager::GetBallUpgrades()
	{
		return currentBallUpgrades;
	}

}
