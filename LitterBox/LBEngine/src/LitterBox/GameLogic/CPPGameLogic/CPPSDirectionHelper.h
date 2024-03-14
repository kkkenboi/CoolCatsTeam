/*!************************************************************************
 * \file				CPPSDirectionHelper.cpp
 * \author(s)			Kenji Brannon Chong
 * \par DP email(s):  	kenjibrannon.c@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This file contains the file declaration for the Direction
 *						Helper which points the player in the general direction
 *						when the game is over.
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once

#include "Platform/Windows/Windows.h"
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Engine/Input.h"
namespace LB
{
	class CPPSDirectionHelper : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief Start function for the Direction Helper
		* It gets the the needed pointers to different components and Game Objects
		* for easy access
		**************************************************************************/
		void Start() override;
		/*!************************************************************************
		* \brief Update function for Direction Helper to update when there is only
		* one enemy left or if the upgrade is not taken yet
		**************************************************************************/
		void Update() override;
		/*!************************************************************************
		* \brief Destroy function for the Direction Helper.
		*
		**************************************************************************/
		void Destroy() override;
		//Caching the Direction Helper's own transform for easy access
		CPTransform* mTransform;
		CPTransform* mPlayerTransform;
		CPTransform* mEventTransform;

		//Caching the CameraFollow GO for easy access
		GameObject* mCameraFollow;
		//Caching the Game Manager GO for easy access
		GameObject* mGameManager;
		//Mouse position for caching
		Vec2<float> MousePos;
		//Vector for calculating the direction to an event, be it last enemy or upgrades
		Vec2<float> DirToEvent;

		bool mLastEnemy{ false };
		bool mUpgradeSpawned{ false };
	};
	REGISTER_SCRIPT(CPPSDirectionHelper)

}

