/*!************************************************************************
 * \file				CPPShield.h
 * \author(s)			Vanessa Chua Siew Jin
 * \par DP email(s):  	vanessasiewjin@digipen.edu
 * \par Course:       	CSD2450A
 * \date				28/02/2024
 * \brief 				This file contains the file declarations for the
 *						Shield
 *
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

		Vec2<float> cachedPosition{};	//store the last shield pos 
		float cachedRot{};				//store the last rotation (rad)

		//Player GameObject, getter and position
		GameObject* mPlayer;
		GameObject* GetPlayer();
		Vec2<float> GetPlayerPos();

		//Vec2<float> DirOfThePlayer;

		float offset{ 75.0f };

		// TODO: REFACTOR, TEMP HACK UNTIL CHILD STARTING IS FIXED
		bool m_ScriptSet{ false };

		CPCollider* GetCollider();

		CPRender* mRender{ nullptr };
		CPRigidBody* mRigidBody{ nullptr };
		CPCollider* mCollider{ nullptr };

		//GameObject* mBall;

		void OnCollisionEnter(CollisionData colData) override;

		//void SetShouldFace(bool state);

		//bool mShouldFace{ false };
	
	};
	REGISTER_SCRIPT(CPPShield)
}

