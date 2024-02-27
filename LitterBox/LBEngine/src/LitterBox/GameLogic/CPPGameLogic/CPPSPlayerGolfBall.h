/*!************************************************************************
 \file				CPPSPlayerGolfBall.cpp
 \author(s)			Vanessa Chua Siew Jin, Kenji Brannon Chong
 \par DP email(s):	vanessasiewjin@digipen.edu, kenjibrannon.c@digipen.edu
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

		/*!***********************************************************************
		\brief
		Getter function for rendering, EB, Collider and the player
		*************************************************************************/
		CPRender* GetRender();
		CPRigidBody* GetRigidBody(); //for RigidBody for the golf ball
		CPCollider* GetCollider(); //for collider for the golf ball
		GameObject* GetHero();

		/*!***********************************************************************
		\brief
		On collision function if it collides with any other colliders
		*************************************************************************/
		void OnCollisionEnter(CollisionData colData);

		//We don't actually need all of them, we only need the ball ones.
		//But the rest are there so you don't wonder why there's jumps in the numbers
		enum BallUpgrades {
			DEFAULT = (1<< 0),
			SPLIT = (1 << 1),		//Ball explodes on contact
			BOMB = (1 << 2),		//Ball splits into 3 (not counted towards total ball count)
			BIGBALL = (1 << 3),		//Ball increase size and mass
			MOVESPEED = (1 << 4),	//Increase ball count by 3	(capped a 9)
			MOREHEALTH = (1<< 5),	//Increase Max health by 1 (capped at 9)
			MOREBALL = (1<<6)		//Increase player movespeed
		};
		

		//Initially ball will have no upgrades
		//This value will be set by the upgrade manager
		BallUpgrades currentBallUpgrades = DEFAULT;

		/*!************************************************************************
		 * \brief Set the Ball Upgrade object
		 * 
		**************************************************************************/
		void SetBallUpgrade(int);

		/*!************************************************************************
		 * \brief Helper function to remove the ball by setting it's lifetime to zero
		 * 
		**************************************************************************/
		void DestroyBall();
		//Upgrade functions
		/*!************************************************************************
		 * \brief Function for handling the ball explosion
		 * 
		**************************************************************************/
		void Explode();

		/*!************************************************************************
		 * \brief Function to handle the ball splitting (empty for now)
		 * 
		**************************************************************************/
		void Split(Vec2<float> forceToApply);
		bool canDestroy{ false };
		bool hasSplit{ false };
	private:
		/*!***********************************************************************
		\brief
		Private variables for rendering, rb colliders, player
		*************************************************************************/
		CPRender* mRender{ nullptr }; //animation purpose
		CPRigidBody* mRigidBody{ nullptr }; //Getting the RB of the enemy
		CPCollider* mCollider{ nullptr }; //Getting the collider of the enemy
		GameObject* mPlayer{ nullptr }; //Getting the game object of the player

		

		//Stats of the ball should have
		float mSpeedMagnitude{}; //speed of the golf ball
		float mVelocity{}; //Velocity of the golf ball
		float mSize{1.0f}; //the size of the golf ball

		//Upgrade stats
		float mExplosionRadius{100.f};
		//Player shoot force is 4500, we lower it slightly 
		float explosionForceMag{2000.f};

		float mLifetime{ 1.5f }, mCurrentLifetime{ 1.5f }; //life time for the golfball to despawn after a certain timing
		Event<> onBallDisappear;
		bool canSplit{ false };	//for the upgrade to decide if the ball can split or not
	};
	//Just some operator overloads to make the ball upgrade enum flags easier to read and understand lol

	/*!************************************************************************
	 * \brief AND operator overload for the ball upgrade enum
	 * 
	 * \param lhs ball upgrade enum
	 * \param rhs ball upgrade enum
	 * \return CPPSPlayerGolfBall::BallUpgrades returns the value of LHS AND RHS
	**************************************************************************/
	inline CPPSPlayerGolfBall::BallUpgrades operator& (CPPSPlayerGolfBall::BallUpgrades lhs, CPPSPlayerGolfBall::BallUpgrades rhs) {
		return static_cast<CPPSPlayerGolfBall::BallUpgrades>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}

	/*!************************************************************************
	 * \brief OR operator overload for the ball upgrade enum
	 * 
	 * \param lhs ball upgrade enum
	 * \param rhs ball upgrade enum
	 * \return CPPSPlayerGolfBall::BallUpgrades returns the value of LHS OR RHS
	**************************************************************************/
	inline CPPSPlayerGolfBall::BallUpgrades operator| (CPPSPlayerGolfBall::BallUpgrades lhs, CPPSPlayerGolfBall::BallUpgrades rhs) {
		return static_cast<CPPSPlayerGolfBall::BallUpgrades>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	/*!************************************************************************
	 * \brief OR ASSIGNMENT operator overload for the ball upgrade enum
	 * 
	 * \param lhs ball upgrade enum reference
	 * \param rhs ball upgrade enum
	 * \return CPPSPlayerGolfBall::BallUpgrades& Sets the value of the enum to be equals LHS OR RHS
	 * (Basically it's like adding 2 enum flag values together)
	**************************************************************************/
	inline CPPSPlayerGolfBall::BallUpgrades& operator|= (CPPSPlayerGolfBall::BallUpgrades& lhs, CPPSPlayerGolfBall::BallUpgrades rhs) {
		return lhs = lhs | rhs;
	}
	REGISTER_SCRIPT(CPPSPlayerGolfBall)
}