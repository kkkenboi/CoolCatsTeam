/*!************************************************************************
 * \file				CPPSUpgradeManager.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This source file contains the implementation for the 
 * 						upgrademanager functions. It handles the whole 
 * 						upgrade system for the game including
 * 						spawning the upgrades
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPSUpgradeManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "CPPSUpgrade.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "CPPGameManager.h"
/*!************************************************************************
 * \brief Small note: By right I think we should have just let the upgrade manager
 * keep track of what upgrades the ball is supposed to have, and the ball should
 * poll and ask what kind of upgrades it has when it spawns or something
**************************************************************************/
namespace LB
{
	/*!************************************************************************
	 * \brief Start function for the game where the upgrade manager is 
	 * initialised. It also decides on the order for the upgrades to spawn.
	 * CURRENT IT IS SEEDED
	**************************************************************************/
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
	/*	for (UpgradeType upgrade : UpgradesList)
		{
			std::cout << static_cast<int>(upgrade) << '\n';
		}*/
		//SpawnUpgrades();
	}

	/*!************************************************************************
	 * \brief Update function for the upgrade manager. Currently does nothing
	 * 
	**************************************************************************/
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

	/*!************************************************************************
	 * \brief Destroy function for the upgrade manager, currently does nothing
	 * 
	**************************************************************************/
	void CPPSUpgradeManager::Destroy()
	{
	}

	/*!************************************************************************
	 * \brief Function to handle spawning upgrades, spawns 3 upgrades if possible
	 * and does the assigning of the upgrade values and the sprites
	**************************************************************************/
	void CPPSUpgradeManager::SpawnUpgrades()
	{	//Forgive me lord for I have sinned :pensive:
		//if no upgrades left we just don't spawn anything
		if (UpgradesList.empty()) return;
		if (UpgradesList.size() < 1) return;		//This check is to know how many to spawn
		leftUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *leftUpgrade);
		leftUpgrade->SetName("leftUpgrade");	//health
		leftUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[0]);
		//Then we also assign the ID based on the upgrades 
		leftUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 1]);
		leftUpgrade->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", UpgradesList[UpgradesList.size() - 1]-1);	//this assigns the sprite texture

		if (UpgradesList.size() < 2) return;	
		middleUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *middleUpgrade);
		middleUpgrade->SetName("middleUpgrade"); //bomb
		middleUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[1]);
		middleUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 2]);
		middleUpgrade->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", UpgradesList[UpgradesList.size() - 2]-1);	

		
		if (UpgradesList.size() < 3) return;
		rightUpgrade = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("Upgrade", *rightUpgrade);
		rightUpgrade->SetName("rightUpgrade");	//more ball
		rightUpgrade->GetComponent<CPTransform>()->SetPosition(UpgradePositions[2]);
		rightUpgrade->GetComponent<CPPSUpgrade>()->AssignUpgradeID(UpgradesList[UpgradesList.size() - 3]);
		rightUpgrade->GetComponent<CPRender>()->SetSpriteTexture("Upgrades", UpgradesList[UpgradesList.size() - 3]-1);	

		//Once we have the sprites, we just set the upgrades by the upgradelist index instead
		//and make sure that it matches up with the sprite sheet index
	
	}

	/*!************************************************************************
	 * \brief Function to remove the upgrade from the pool of upgrades
	 * 
	 * \param upgradeType Upgrade to remove from pool
	**************************************************************************/
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

	/*!************************************************************************
	 * \brief Function to remove all the other upgrades from the screen. The 
	 * selected upgrade will be removed from the pool
	 * \param chosen The upgrade to remove from the list that was chosen by the player
	**************************************************************************/
	void CPPSUpgradeManager::HideUpgrades(int chosen)
	{
		//When the upgrade is picked up we want to tell the game manager to start the
		//next wave
		GameObject* GameManagerObj = GOMANAGER->FindGameObjectWithName("GameManager");
		GameManagerObj->GetComponent<CPPSGameManager>()->NextWave();
		//Once we pick the upgrade we have to remove it from the pool
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
	/*!************************************************************************
	 * \brief Function to set the ball's upgrade
	 * 
	 * \param upgradeType Upgrade to set
	**************************************************************************/
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

	/*!************************************************************************
	 * \brief Function to get the current ball upgrades
	 * 
	 * \return int Upgrades that the ball should currently have 
	**************************************************************************/
	int CPPSUpgradeManager::GetBallUpgrades()
	{
		return currentBallUpgrades;
	}

}
