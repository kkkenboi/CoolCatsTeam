#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include <vector>
#include <random>
namespace LB
{
	enum UpgradeType
	{
		BOMB = 1,	//Ball explodes on contact
		SPLIT,		//Ball splits into 3 (not counted towards total ball count)
		BIGBALL,	//Ball increase size and mass
		MOREBALL,	//Increase ball count by 3	(capped a 9)
		MOREHEALTH,	//Increase Max health by 1 (capped at 9)
		MOVESPEED,	//Increase player movespeed
		MAXCOUNT
		
	};

	class CPPSUpgradeManager : public CPPBehaviour
	{
	public:
		void Start() override;
		void Update() override;
		void Destroy() override;

		void SpawnUpgrades(); //Function to spawn 3 upgrades from list of upgrades
		void HideUpgrades();

		//Really scuffed way of making upgrades
		std::vector<UpgradeType> UpgradesList;
	private:
	};
	REGISTER_SCRIPT(CPPSUpgradeManager)

}