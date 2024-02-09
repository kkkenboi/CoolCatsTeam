/*!************************************************************************
 \file				CPPSMage.h
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao, Amadeus Chia
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan@digipen.edu, amadeusjinhan.chia@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the CPPSCMage class and all its functionalities,
it handls the logic for the Mage enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPSBaseEnemy.h"
#include "CPPSBaseGolfBall.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB
{
	// Use GameObj to access the gameobj of the class
	class CPPSMage : public CPPSBaseEnemy
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;
		void Hurt() override;


		//getter functions //inherited now
		/*virtual CPRender* GetRender() override;
		virtual CPRigidBody* GetRigidBody() override;
		virtual CPCollider* GetCollider() override;
		virtual GameObject* GetHero() override;*/
		GameObject* GetProjectile();

		//spawn function
		void SpawnProjectile();

		void OnCollisionEnter(CollisionData colData) override;
		void Die() override;

		//stats of what the mage will have
		// inherited now
		//int mHealth; //health of the mage
		//float mSpeedMagnitude{}; //normal speed when approaching towards the player
		float mBackOffSpeed{}; //When player is nearby, it backs off

		//------------------CHASE STATE------------------
		float mAttackCooldown{}, mAttackCooldownCurrent{};
		float mTooClose{};

		//------------------SHOOTING STATE------------------
		float mAttackRange{};
		int mNumOfProjectile{}, mNumOfProjectileCurrent{}; //the number of projectile the player will shoot
		float mProjCooldown{}, mProjCooldownCurrent{};
		float mProjSpeed{};

		float mGotAttacked{}, mGotAttackedCooldown{};

		//------------------BACKOFF STATE------------------
		//Boundaries between enemy and player
		float mMinDistance{}, mMaxDistance{}, mBackOffDistance{};
		float rangeDistance{};

		//------------------Rendering,RB,Collider------------------
		//CPRender* mRender; //animation purpose
		//CPRigidBody* mRigidBody; //Getting the RB of the enemy
		//CPCollider* mCollider; //Getting the collider of the enemy

	private:
		//GameObject* mPlayer{ nullptr }; //Getting the player GameObject
		GameObject* mProjectile{ nullptr }; //Getting the projectile GameObject
		//Vec2<float> leftFace;
		//Vec2<float> rightFace;

		// Holds the different state and the current state
		// that the Mage enemy is in
		FiniteStateMachine mFSM{}; //state machine of the mage enemy, I will have to add into it

		bool mInitialised{ false }; //setting to false, later after everything has been initialised in the cpp, it will set to true
	};
	REGISTER_SCRIPT(CPPSMage)

	// States
	//STATES : IDLE, CHASING, BACKOFF, HURT, SHOOTING
	/*!***********************************************************************
	\brief
	Idle state of the Mage enemy
	*************************************************************************/
	class MageIdleState : public State
	{
	public:
		MageIdleState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Chase state of the Mage enemy
	*************************************************************************/
	class MageChaseState : public State
	{
	public:
		MageChaseState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	BackOff state of the Mage enemy
	*************************************************************************/
	class MageBackOffState : public State
	{
	public:
		MageBackOffState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Hurt state of the Mage enemy
	*************************************************************************/
	class MageHurtState : public State
	{
	public:
		MageHurtState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Shooting state of the Mage enemy
	*************************************************************************/
	class MageShootingState : public State
	{
	public:
		MageShootingState(CPPSMage* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSMage* mEnemy;
	};
}