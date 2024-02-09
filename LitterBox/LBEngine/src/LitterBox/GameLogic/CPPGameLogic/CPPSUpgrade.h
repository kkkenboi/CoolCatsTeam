/*!************************************************************************
 * \file				CPPSUpgrade.h
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This header file contains the function declarations
 * 						for the upgrade prefab
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "CPPSUpgradeManager.h"
namespace LB
{
	class CPPSUpgrade : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief Start function for the upgrade (Currently does nothing)
		* 
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief Update function for the upgrade, currently checks if 
		* the object should be destroyed or not
		**************************************************************************/
		void Update() override;
		/*!************************************************************************
		* \brief Destroy function for the upgrade, currently does nothing
		* 
		**************************************************************************/
		void Destroy() override;

		/*!************************************************************************
		* \brief Function that handles the collision with the upgrade
		* 
		* \param colData Collision data of the collision with the upgrade
		**************************************************************************/
		void OnCollisionEnter(CollisionData) override;

		/*!************************************************************************
		* \brief Function to assign the upgrade, it gets called by the upgrade manager
		* and it assigns the current type of upgrade to the ball
		* \param upgradeType Type of upgrade to assign
		**************************************************************************/
		void AssignUpgradeID(int);
		//Upgrade assigned by the upgrade managaer
		int assignedUpgradeType{ 0 };
		bool canDestroy{ false };
	private:
		//GameObject* BallGameObj{ nullptr };
	};
	REGISTER_SCRIPT(CPPSUpgrade)
}

