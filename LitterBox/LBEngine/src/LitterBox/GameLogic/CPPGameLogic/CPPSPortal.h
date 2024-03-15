/*!************************************************************************
 * \file				CPPGameManager.h
 * \author(s)			Amadeus Chia
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2451A
 * \date				09/02/2024
 * \brief 				This header file contains the declarations for the
 * 						game manager that handles the AI director of the game
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"
namespace LB
{
	class CPPSPortal : public CPPBehaviour
	{
	public:
		/*!************************************************************************
		* \brief
		* Component functions for the Game Manager
		**************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;
		void OnCollisionEnter(CollisionData colData) override;


	private:
		GameObject* mPlayer{ nullptr };
		GameObject* mGameManager{ nullptr };
		//this is NOT the portal! it's the black circle!
		//I'm using this black circle to do the transition effect!!!
		GameObject* mPortalCenter{ nullptr };
		Vec2<float> mCachedPlayerPos{};
		bool isTransitioning{};
		bool finishTransition{};
		bool temp{}; //this is a temp bool for a temp fix
		bool expandOut{ true };	//this bool is to toggle the circle expanding out or in
		bool playPortalSound{ false };
		//roughly how long it'll take for the player to fly to centre of black hole
		//float lerpDuration{1.f};	commented out for now because I'm hacking in the lerp
		float rotAnglePerSec{ 720.f };
		float timer{};		//generic timer for the lerp
		float rotTimer{};	//separate timer for the rotation
		float rotAngle{ 0 };
		float circleTimer{};	//separate timer for the circle expansion
		float intermissionTimer{};
		float intermissionDuration{ 2.f };

	};
	REGISTER_SCRIPT(CPPSPortal)
}