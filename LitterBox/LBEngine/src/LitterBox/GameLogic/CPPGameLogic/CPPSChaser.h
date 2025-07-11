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
#include "CPPSBaseEnemy.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB 
{
	// Use GameObj to access the gameobj of the class
	class CPPSChaser : public CPPSBaseEnemy 
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void FixedUpdate() override;
		void Update() override;
		void Destroy() override;
		void Hurt() override;

		void OnCollisionEnter(CollisionData colData) override;
		void OnCollisionStay(CollisionData colData) override;
		void OnCollisionExit(CollisionData colData) override;

		void Die() override;

		/*!***********************************************************************
		\brief
		Getters for the individual components of the Chaser
		*************************************************************************/
		//CPRender* GetRender();
		//CPRigidBody* GetRigidBody();
		//CPCollider* GetCollider();
		//GameObject* GetHero();
		//int& GetHealth();
		//float& GetSpeedMag();	//Getter function for the speed of the chaser
		float& GetHurtTimer();
		//GameObject* chaserClubCol{};

		CPAnimator* m_moveAnimator{ nullptr };
		GameObject* m_AudioManager{ nullptr };
		CPAudioSource* m_AudioSource{ nullptr };
	private:

		// Holds the different state and the current state
		// that the Chaser enemy is in
		FiniteStateMachine mFSM{};

		//int mHealth{}; //health of the chaser
		GameObject *m_club{ nullptr };
		//float mSpeedMagnitude{}; //speed
		float mHurtTimer{}; //hurt timer

		//for the ball collision
		float mGotAttacked{0.5f}, mGotAttackedCooldown{0.0f};

		bool mInitialised{ false }, clubFacingLeft{ false };
	};
	REGISTER_SCRIPT(CPPSChaser)


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
		void FixedUpdate() override;
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
		void FixedUpdate() override;
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
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSChaser* mEnemy;
	};

}