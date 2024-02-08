#include "CPPSUpgradeManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "CPPSUpgrade.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "CPPGameManager.h"
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
		// Current order is : morehealth, split, movespeed,bomb,moreball,bigball
		//514263, but because we're doing pop back, it goes from the back
		auto rngesus = std::default_random_engine{};
		std::shuffle(UpgradesList.begin(), UpgradesList.end(), rngesus);
		//Debug info
		/*for (UpgradeType upgrade : UpgradesList)
		{
			std::cout << static_cast<int>(upgrade) << '\n';
		}*/
		//SpawnUpgrades();
	}

	void CPPSUpgradeManager::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_U))
		{
			//Renderer::GRAPHICS->shake_camera();
		//	SpawnUpgrades();
		}
		
		//if (!isSpawned) {
		//	//isSpawned = true;
		//	//We can't put this in the start because the components haven't all been initialised yet.
		//	//There's definitely a better way to do this...
		//	if(!UpgradesList.empty())
		//	GOMANAGER->FindGameObjectWithName("leftUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size()-1]);
		//	//UpgradesList.pop_back();

		//	if (!UpgradesList.empty())
		//	GOMANAGER->FindGameObjectWithName("middleUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 1]);
		//	//UpgradesList.pop_back();

		//	if (!UpgradesList.empty())
		//	GOMANAGER->FindGameObjectWithName("rightUpgrade")->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 1]);
		//	//UpgradesList.pop_back();
		//}
	}

	void CPPSUpgradeManager::Destroy()
	{
	}

	void CPPSUpgradeManager::SpawnUpgrades()
	{
		//if no upgrades left we just don't spawn anything
		if (UpgradesList.empty()) return;
		if (UpgradesList.size() < 1) return;		//This check is to know how many to spawn
		leftUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *leftUpgrade);
		leftUpgrade->SetName("leftUpgrade");	//health
		leftUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[0]);
		leftUpgrade->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", UpgradesList[UpgradesList.size() - 1]);	//this assigns the sprite texture
		//Then we also assign the ID based on the upgrades 
		leftUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 1]);

		if (UpgradesList.size() < 2) return;	
		middleUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *middleUpgrade);
		middleUpgrade->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", UpgradesList[UpgradesList.size() - 2]);	//Explosive sprite
		middleUpgrade->SetName("middleUpgrade"); //bomb
		middleUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[1]);
		middleUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 2]);

		
		if (UpgradesList.size() < 3) return;
		rightUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *rightUpgrade);
		rightUpgrade->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", UpgradesList[UpgradesList.size() - 3]);	//Health sprite
		rightUpgrade->SetName("rightUpgrade");	//more ball
		rightUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[2]);
		rightUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 3]);


		//Once we have the sprites, we just set the upgrades by the upgradelist index instead
		//and make sure that it matches up with the sprite sheet index
	
	}

	void CPPSUpgradeManager::RemoveUpgradeFromList(UpgradeType upgradeType)
	{
		for (auto it{ UpgradesList.begin() }; it != UpgradesList.end(); ++it)
		{
			if (*it == upgradeType)
			{
				UpgradesList.erase(it);
				break;
			}
		}
	}

	void CPPSUpgradeManager::HideUpgrades(int chosen)
	{
		GameObject* GameManagerObj = GOMANAGER->FindGameObjectWithName("GameManager");
		
		GameManagerObj->GetComponent<CPPSGameManager>()->NextWave();
		
		RemoveUpgradeFromList(static_cast<UpgradeType>(chosen));
		if (leftUpgrade != nullptr)
		{
			leftUpgrade->GetComponent<CPPSUpgrade>()->canDestroy = true;
			leftUpgrade = nullptr;
		}
		if (middleUpgrade != nullptr)
		{
			middleUpgrade->GetComponent<CPPSUpgrade>()->canDestroy = true;
			middleUpgrade = nullptr;

		}
		
		if (rightUpgrade != nullptr)
		{
			rightUpgrade->GetComponent<CPPSUpgrade>()->canDestroy = true;
			rightUpgrade = nullptr;
		}

		/*for (auto it{ UpgradesList.begin() }; it != UpgradesList.end(); ++it)
		{
			std::cout <<"Upgrades left " << *it << '\n';
		}*/
	}

	void CPPSUpgradeManager::SetBallUpgrade(int upgradeType)
	{
		//This gets called by the upgrade when it gets hit
		//We just use it as ints for now to set ball upgrades
		//std::cout << "upgrade type set " << upgradeType << '\n';
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
