#include "CPPSUpgradeManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "CPPSUpgrade.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Serialization/AssetManager.h"
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
		SpawnUpgrades();
	}

	void CPPSUpgradeManager::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_H))
		{
			HideUpgrades();
		}
		if (!isSpawned) {
			isSpawned = true;
			GOMANAGER->FindGameObjectWithName("leftUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(MOREBALL);
			GOMANAGER->FindGameObjectWithName("middleUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(BOMB);
			GOMANAGER->FindGameObjectWithName("rightUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(MOREHEALTH);
		}
	}

	void CPPSUpgradeManager::Destroy()
	{
	}

	void CPPSUpgradeManager::SpawnUpgrades()
	{
		leftUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *leftUpgrade);

		middleUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *middleUpgrade);

		rightUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *rightUpgrade);


		leftUpgrade->SetName("leftUpgrade");	//health
		leftUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[0]);
		SpriteSheet selectedsheet = ASSETMANAGER->SpriteSheets["Upgrades"];
		leftUpgrade->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->GetTextureUnit(selectedsheet.GetPNGRef()),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->width / selectedsheet.Sprites().size(),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->height / selectedsheet.Sprites().size(),
			selectedsheet.Sprites()[0].m_min, selectedsheet.Sprites()[0].m_max);

		middleUpgrade->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->GetTextureUnit(selectedsheet.GetPNGRef()),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->width / selectedsheet.Sprites().size(),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->height / selectedsheet.Sprites().size(),
			selectedsheet.Sprites()[1].m_min, selectedsheet.Sprites()[1].m_max);
		middleUpgrade->SetName("middleUpgrade"); //bomb
		middleUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[1]);

		rightUpgrade->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->GetTextureUnit(selectedsheet.GetPNGRef()),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->width / selectedsheet.Sprites().size(),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->height / selectedsheet.Sprites().size(),
			selectedsheet.Sprites()[2].m_min, selectedsheet.Sprites()[2].m_max);
		rightUpgrade->SetName("rightUpgrade");	//more ball
		rightUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[2]);
		//
		//CPPSUpgrade* test = GOMANAGER->FindGameObjectWithName("leftUpgrade")->GetComponent<CPPSUpgrade>();
		//if (test == nullptr) std::cout << "joemam\n";
		//->AssignUpgradeID(UpgradesList[0]);
		// GOMANAGER->FindGameObjectWithName("leftUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[0]);
		//GOMANAGER->FindGameObjectWithName("middleUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[0]);
		//GOMANAGER->FindGameObjectWithName("rightUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[0]);

		//Then we assign the upgrade types to the upgrades 
		//probably can't hard code the upgrade list because it reduces over time. 
		//Might need to use like a stack or something and pop/push valuesS in and out
		/*leftUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID((int)(UpgradesList[0]));*/
		//middleUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[1]);
		//rightUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[2]);
	}

	void CPPSUpgradeManager::HideUpgrades()
	{
		if (leftUpgrade) 
		{
			GOMANAGER->RemoveGameObject(leftUpgrade);
			leftUpgrade = nullptr;
		}
		if (middleUpgrade)
		{
			GOMANAGER->RemoveGameObject(middleUpgrade);
			middleUpgrade = nullptr;
		}
		if (rightUpgrade)
		{
			GOMANAGER->RemoveGameObject(rightUpgrade);
			rightUpgrade = nullptr;
		}
	}

	void CPPSUpgradeManager::SetBallUpgrade(int upgradeType)
	{
		//This gets called by the upgrade when it gets hit
		//We just use it as ints for now to set ball upgrades
		std::cout << "upgrade type set " << upgradeType << '\n';
		currentBallUpgrades |= (1 << upgradeType);
		//Then we loop through and remove the upgrade that we got from the list
		/*for (std::vector<UpgradeType>::iterator it = UpgradesList.end(); it != UpgradesList.begin();)
		{
			if (*it == static_cast<UpgradeType>(upgradeType))
			{
				auto itToRemove = it--;
				UpgradesList.erase(itToRemove);
			}
			else
			{
				--it;
			}
		}*/
	}

	int CPPSUpgradeManager::GetBallUpgrades()
	{
		return currentBallUpgrades;
	}

}
