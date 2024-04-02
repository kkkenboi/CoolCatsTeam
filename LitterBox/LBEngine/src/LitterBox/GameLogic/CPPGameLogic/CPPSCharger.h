/*!************************************************************************
 \file				CPPSCharger.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				27-02-2024
 \brief
 This file contains the CPPSCharger class and all its functionalities,
it handles the logic for the Charger enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "CPPSBaseEnemy.h"
#include "LitterBox/GameLogic/StateMachine.h"

namespace LB
{
	class CPPSCharger : public CPPSBaseEnemy //inherit from base enemy
	{
	public:
		//from base
		void Start() override;
		void Update() override;
		void Destroy() override;
		void FixedUpdate() override;

		void OnCollisionEnter(CollisionData colData) override;
		
		//Hurt and Die from base, overide
		void Hurt() override;
		void Die() override;
		
		void SetShouldFace(bool state);
		
		//Getter and Helper
		Vec2<float> GetPlayerPos();
		Vec2<float> GetChargerPos();
		Vec2<float> DirBToA(Vec2<float> a, Vec2<float> b);

		//*********************************************************

		//------------------IDLE STATE------------------
		float mIdleCooldown{};

		//------------------MOVE STATE------------------
		float mDistToWindUp{};

		//------------------HURT STATE------------------
		float mTimerDurationHurt{};

		//------------------WINDUP STATE------------------
		float mTimerToCharge{};

		//------------------CHARGE STATE------------------
		float mChargingSpeed{};
		Vec2<float> mChargeDirection{};
		Vec2<float> mChargeNormalForce{};

		//------------------CHARGE STATE------------------
		float mTimerWhenStunned{}, mStunTimerElapsed{}, mStunStopMovingElapsed{};//timer

		//*********************************************************

		bool isChargerDead{ false }; //check if charger is dead

		CPParticle* FootstepsParticle{ nullptr }; //*WallImpactParticle{ nullptr }; //Foot Particles

		CPAnimator* mDizzyAnim{ nullptr }, * mAngerAnim{ nullptr }, * mPuffAnim{ nullptr }, * mMoveAnim{ nullptr }, * mAngerTwoAnim{ nullptr }; //Animation

		float m_FootstepsParticleEmitRate{}, m_WallImpactParticleEmitRate{};


		CPTransform* mTransform{ nullptr }; //trans of the GO

		//Charger's Dizzy Effects
		GameObject* mDizzyObj{ nullptr }; //will be spawnned when got stunned
		CPTransform* mDizzyObjTrans{ nullptr };
		CPRender* mDizzyRender{ nullptr };

		//Charger's Anger Effects
		GameObject* mAngerObj{ nullptr }; //will be spawnned when got stunned
		CPTransform* mAngerObjTrans{ nullptr };
		CPRender* mAngerRender{ nullptr };

		GameObject* mAngerTwoObj{ nullptr }; //will be spawnned when got stunned
		CPTransform* mAngerObjTwoTrans{ nullptr };
		CPRender* mAngerTwoRender{nullptr};

		//bool check for shield
		bool m_isCharging{  }, m_isStunned{  }, m_isHurt{  }, m_isLocked{  };
		// A helper function to change state in the shield
		void ChangeToStunned();

	private:
		FiniteStateMachine mFSM{};
		//GameObject* mWall;
		bool mInitialised{ false }; //setting to false, later after everything has been initialised in the cpp, it will set to true
		
		//stats
		//float mChargeSpeed;

	};
	REGISTER_SCRIPT(CPPSCharger)

	//************************************************************************************************
	// Idle
	//************************************************************************************************
	class ChargerIdleState : public State
	{
	public:
		ChargerIdleState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSCharger* mEnemy;
	};

	//************************************************************************************************
	// Move
	//************************************************************************************************
	class ChargerMoveState : public State
	{
	public:
		ChargerMoveState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSCharger* mEnemy;
	};

	//************************************************************************************************
	// Hurt
	//************************************************************************************************
	class ChargerHurtState : public State
	{
	public:
		ChargerHurtState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSCharger* mEnemy;
	};

	//************************************************************************************************
	// WindUp
	//************************************************************************************************
	class ChargerWindUpState : public State
	{
	public:
		ChargerWindUpState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSCharger* mEnemy;
	};

	//************************************************************************************************
	// Charge
	//************************************************************************************************
	class ChargerChargeState : public State
	{
	public:
		ChargerChargeState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSCharger* mEnemy;
	};

	//************************************************************************************************
	// Stunned
	//************************************************************************************************
	class ChargerStunnedState : public State
	{
	public:
		ChargerStunnedState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name);
		void Enter() override;
		void FixedUpdate() override;
		void Update() override;
		void Exit() override;
	private:
		CPPSCharger* mEnemy;
	};

}