/*!************************************************************************
 \file				CPPSPlayerGolfBall.cpp
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the CPPSPlayerGolfBall, behaviour of the player's golf ball

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPBehaviour.h"
#include "Litterbox/Factory/Components.h"

namespace LB
{
	class CPPSPlayerGolfBall : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		CPRender* GetRender();
		CPRigidBody* GetRigidBody(); //for RigidBody for the golf ball
		CPCollider* GetCollider(); //for collider for the golf ball
		GameObject* GetHero();

		void OnCollisionEnter(CollisionData colData);

		//timer to despawn the golfball
		//destroy the golfball
	private:
		CPRender* mRender{ nullptr }; //animation purpose
		CPRigidBody* mRigidBody{ nullptr }; //Getting the RB of the enemy
		CPCollider* mCollider{ nullptr }; //Getting the collider of the enemy
		GameObject* mPlayer{ nullptr };

		//Stats of the ball should have
		float mSpeedMagnitude{};
		float mVelocity{};
		float mSize{};

		float mLifetime{ 1.5f }, mCurrentLifetime{ 1.5f };
	};
}