/*!************************************************************************
 \file				CPPSChaser.h
 \author(s)			Justine Carlo Villa Ilao, Vanessa Chua Siew Jin
 \par DP email(s):	justine.c@digipen.edu, vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the CPPSChaser class and all its functionalities,
it handls the logic for the chaser enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "CPPBehaviour.h"
#include "Litterbox/Factory/Components.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB 
{
	// Use GameObj to access the gameobj of the class
	class CPPSChaser : public CPPBehaviour 
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		void OnCollisionEnter(CollisionData colData) override;


		/*!***********************************************************************
		\brief
		Getters for the individual components of the Chaser
		*************************************************************************/
		CPRender* GetRender();
		CPRigidBody* GetRigidBody();
		CPCollider* GetCollider();
		GameObject* GetHero();
		int& GetHealth();
		float& GetSpeedMag();	//Getter function for the speed of the chaser
		float& GetHurtTimer();

	private:


		CPRender* mRender;
		CPRigidBody* mRigidBody;
		CPCollider* mCollider;

		GameObject* mPlayer{ nullptr };

		// Holds the different state and the current state
		// that the Chaser enemy is in
		FiniteStateMachine mFSM{};

		int mHealth{};
		float mSpeedMagnitude{};
		float mHurtTimer{};

		bool mInitialised{false};
	};

	// States
	/*!***********************************************************************
	\brief
	Idle state of the chaser enemy
	*************************************************************************/
	class IdleState : public State
	{
	public:
		IdleState(CPPSChaser* enemy_ptr ,FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Chase state of the chaser enemy (Basic seeking behaviour)
	*************************************************************************/
	class ChaseState : public State
	{
	public:
		ChaseState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

	/*!***********************************************************************
	\brief
	Hurt state of the chaser enemy
	*************************************************************************/
	class HurtState : public State
	{
	public:
		HurtState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

}