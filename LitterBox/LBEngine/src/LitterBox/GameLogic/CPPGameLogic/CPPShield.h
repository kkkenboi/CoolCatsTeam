/*!************************************************************************
 * \file				CPPSAimUI.h
 * \author(s)			Amadeus Chia
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2450A
 * \date				09/02/2024
 * \brief 				This file contains the file declarations for the
 * 						Aim script. (Basically points UI to mouse cursor)
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "CPPSBaseEnemy.h"
#include "CPPSCharger.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPShield : public CPPBehaviour
	{
		void Start() override;

		void Update() override;

		void Destroy() override;

		//Cachin the Shield's own transform for easy code access
		CPTransform* mTransform;
		//Caching the charger GO for easy access
		
		//Charger GameObject, getter and position
		GameObject* mCharger;
		CPPSCharger* mChargerScript;
		GameObject* GetCharger();
		Vec2<float> GetChargerPos();

		//Player GameObject, getter and position
		GameObject* mPlayer;
		GameObject* GetPlayer();
		Vec2<float> GetPlayerPos();

		//Vec2<float> DirOfThePlayer;
		bool mLock{ false };

		float offset{ 75.0f };

		CPCollider* GetCollider();


		CPRender* mRender{ nullptr };
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };

	
	
	};
	REGISTER_SCRIPT(CPPShield)
}

