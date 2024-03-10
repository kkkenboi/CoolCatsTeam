/*!************************************************************************
 * \file				CPPSUpgradeManager.h
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This source file contains the function declarations
 * 						for the player's ball. It basically handles the 
 * 						sort of upgrades and effects the ball would have
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include <vector>
#include <random>
namespace LB
{
	/*!************************************************************************
	 * \brief Enum for the upgrades
	 * 
	**************************************************************************/
	enum UpgradeType
	{
		SPLIT = 1,		//Ball splits into 3 (not counted towards total ball count)
		BOMB,			//Ball explodes on contact
		BIGBALL,		//Ball increase size and mass
		MOVESPEED,		//Increase player movespeed
		POTION,			//Heal player
		MOREHEALTH,		//Increase Max health by 1 (capped at 6/9)
		MOREBALL,		//Increase ball count by 3	(capped at 6/9)
		MAXCOUNT		//Enum to keep track of the count of upgrades
	};

	class CPPSUpgradeManager : public CPPBehaviour 
	{
	public:
		/*!************************************************************************
		* \brief Start function for the game where the upgrade manager is 
		* initialised. It also decides on the order for the upgrades to spawn.
		* CURRENT IT IS SEEDED
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief Update function for the upgrade manager. Currently does nothing
		* 
		**************************************************************************/
		void Update() override;
		/*!************************************************************************
		* \brief Destroy function for the upgrade manager, currently does nothing
		* 
		**************************************************************************/
		void Destroy() override;

		/*!************************************************************************
		* \brief Function to handle spawning upgrades, spawns 3 upgrades if possible
		* and does the assigning of the upgrade values and the sprites
		**************************************************************************/
		void SpawnUpgrades(); //Function to spawn 3 upgrades from list of upgrades
		/*!************************************************************************
		* \brief Function to remove the upgrade from the pool of upgrades
		* 
		* \param upgradeType Upgrade to remove from pool
		**************************************************************************/
		void RemoveUpgradeFromList(UpgradeType upgradeType);
		/*!************************************************************************
		* \brief Function to remove all the other upgrades from the screen. The 
		* selected upgrade will be removed from the pool
		* \param chosen The upgrade to remove from the list that was chosen by the player
		**************************************************************************/
		void HideUpgrades(int chosen);
		/*!************************************************************************
		* \brief Function to set the ball's upgrade
		* 
		* \param upgradeType Upgrade to set
		**************************************************************************/
		void SetBallUpgrade(int upgradeType);
		/*!************************************************************************
		* \brief Function to get the current ball upgrades
		* 
		* \return int Upgrades that the ball should currently have 
		**************************************************************************/
		int GetBallUpgrades();

		/*!************************************************************************
		* \brief Function to set whether upgrades are spawned
		*
		* \param Whether spawned or not
		**************************************************************************/
		void SetSpawned(bool spawned);

		//Really scuffed way of making upgrades
		//Upgrade manager assigns the upgrade type to the upgrade
		//then the upgrade tells the upgrade manager which upgrades
		//have been picked, and the upgrade manager tells the 
		//ball what upgrades it has when it spawns
		//note: maybe use a stack instead, randomise the values before putting in
		//then use push and pop 
		std::vector<UpgradeType> UpgradesList;
		std::vector<UpgradeType> PlayerUpgradesList;
		//Default is 0 for no upgrades
		int currentBallUpgrades{0};
		bool hasUpgraded{ false };

		//Event for when player gets a NEW upgrade
		Event<UpgradeType> onNewUpgrade{};
	private:
		//Temporary upgrade positions for now, in the future these will be calculated 
		//and placed at the "end point" of the bigger map
		Vec2<float> UpgradePos{};
		std::vector<Vec2<float>> UpgradePositions{ {650.f,785.f}, {950.f,785.f} ,{1250.f,785.f} };
		GameObject* leftUpgrade{ nullptr };
		GameObject* middleUpgrade{ nullptr };
		GameObject* rightUpgrade{ nullptr };
		bool isSpawned{ false };
	};
	REGISTER_SCRIPT(CPPSUpgradeManager)

}