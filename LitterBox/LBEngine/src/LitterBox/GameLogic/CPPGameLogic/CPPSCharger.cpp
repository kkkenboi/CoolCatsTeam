/*!************************************************************************
 \file				CPPSCharger.cpp
 \author(s)			Vanessa Chua Siew Jin, Ryan Tan Jian Hao
 \par DP email(s):	vanessasiewjin@digipen.edu, ryanjianhao.tan@digipen.edu
 \par Course:		CSD2401A
 \date				27-02-2024
 \brief
 This file contains the CPPSCharger class and all its functionalities,
it handles the logic for the Mage enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSCharger.h"
#include "LitterBox/Physics/PhysicsMath.h"

namespace LB
{
	void CPPSCharger::Start()
	{
		CPPSBaseEnemy::Start();

		ChargerIdleState* IDLESTATE = DBG_NEW ChargerIdleState(this, mFSM, "Idle");
		ChargerMoveState* MOVESTATE = DBG_NEW ChargerMoveState(this, mFSM, "Move");
		ChargerHurtState* HURTSTATE = DBG_NEW ChargerHurtState(this, mFSM, "Hurt");
		ChargerWindUpState* WINDUPSTATE = DBG_NEW ChargerWindUpState(this, mFSM, "WindUp");
		ChargerChargeState* CHARGESTATE = DBG_NEW ChargerChargeState(this, mFSM, "Charge");
		ChargerStunnedState* STUNNEDSTATE = DBG_NEW ChargerStunnedState(this, mFSM, "Stunned");

		//STATES : IDLE, MOVE, HURT, WINDUP, CHARGE, STUNNED
		mFSM.AddState(IDLESTATE);
		mFSM.AddState(MOVESTATE);
		mFSM.AddState(HURTSTATE);
		mFSM.AddState(WINDUPSTATE);
		mFSM.AddState(CHARGESTATE);
		mFSM.AddState(STUNNEDSTATE);

		//set current state of Mage to be on idle
		mFSM.SetCurrentState("Idle");

		//set stats on charger
		mHealth = 3;
		mSpeedMagnitude = 50000.f; //speed of movement

		/********************************************/
		// Idle State variables
		/********************************************/
		mIdleCooldown = 2.0f;

		/********************************************/
		// Move State variables
		/********************************************/
		mDistToWindUp = 700.0f;

		/********************************************/
		// WindUp State variables
		/********************************************/
		mTimerToCharge = 3.0f;

		/********************************************/
		// Charge State variables
		/********************************************/
		mChargingSpeed = 100000.f;

		//mAttackCooldown = 2.0f;
		//mAttackCooldownCurrent = 0.0f;
	}

	void CPPSCharger::Update()
	{
		mFSM.Update();
	}

	void CPPSCharger::Destroy()
	{
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Move");
		delete mFSM.GetState("Hurt");
		delete mFSM.GetState("WindUp");
		delete mFSM.GetState("Charge");
		delete mFSM.GetState("Stunned");
	}

	void CPPSCharger::OnCollisionEnter(CollisionData colData)
	{
		CPPSBaseEnemy::OnCollisionEnter(colData);
		if (colData.colliderOther->m_gameobj->GetName() == "ball") 
		{
				mFSM.ChangeState("Hurt");
				Hurt();
		}
	}

	void CPPSCharger::Hurt()
	{
		CPPSBaseEnemy::Hurt();
	}

	void CPPSCharger::Die()
	{
		CPPSBaseEnemy::Die();
		//Code to play death anim goes here
	}

	Vec2<float> CPPSCharger::GetPlayerPos() //get curr player pos
	{
		return GetHero()->GetComponent<CPRigidBody>()->getPos();
	}

	Vec2<float> CPPSCharger::GetChargerPos()
	{
		return GetRigidBody()->getPos();
	}

	Vec2<float> CPPSCharger::DirBToA(Vec2<float> a, Vec2<float> b)
	{
		return (a - b).Normalise();
	}

	/*!***********************************************************************
	\brief
	Idle state of the Charger enemy
	*************************************************************************/
	ChargerIdleState::ChargerIdleState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name) : 
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChargerIdleState::Enter()
	{
		this->Update();
	}

	void ChargerIdleState::Update()
	{
		//this->Update();
		//NEED TO CALCULATE THE DISTANCE BWN THE CHARGER AND THE PLAYER BECOS THE MAP IS GG TO BE BIGGER!!!!
		mEnemy->mIdleCooldown -= static_cast<float>(TIME->GetDeltaTime());
		if (mEnemy->mIdleCooldown <= 0.0f)
		{
			GetFSM().ChangeState("Move");
		}
	}

	void ChargerIdleState::Exit(){ }

	//STATES : IDLE, MOVE, HURT, WINDUP, CHARGE, STUNNED
	/*!***********************************************************************
	\brief
	Move state of the Charger enemy
	*************************************************************************/
	ChargerMoveState::ChargerMoveState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChargerMoveState::Enter()
	{
		DebuggerLogWarning("CHARGER MOVE STATE");
		this->Update();

	}

	void ChargerMoveState::Update()
	{
		//charger will walk slowly towards the player
		//when charger is near the player, state will change to windup
		float DistInBwn = Vec2<float>::Distance(mEnemy->GetChargerPos(), mEnemy->GetPlayerPos());
		
		DebuggerLogWarning("CHARGER MOVE STATE");
		//DebuggerLogFormat("Dist In Between enemy and Hero: %f", DistInBwn);

		Vec2<float> Direction = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());

		Vec2<float> NormalForce = Direction * mEnemy->mSpeedMagnitude;

		if (DistInBwn <= mEnemy->mDistToWindUp)
		{
			GetFSM().ChangeState("WindUp");
		}
		else
		{
			//walk towards the player
			mEnemy->GetRigidBody()->addForce(NormalForce * static_cast<float>(TIME->GetDeltaTime())); //add force to move

		}
		

	}

	void ChargerMoveState::Exit() { }

	/*!***********************************************************************
	\brief
	Hurt state of the Charger enemy
	*************************************************************************/
	ChargerHurtState::ChargerHurtState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChargerHurtState::Enter()
	{
		DebuggerLogWarning("CHARGER HURT STATE");
		this->Update();
	}

	void ChargerHurtState::Update()
	{
		DebuggerLogWarning("CHARGER HURT STATE");
		//this->Update();
	}

	void ChargerHurtState::Exit()
	{

	}

	/*!***********************************************************************
	\brief
	WindUp state of the Charger enemy
	*************************************************************************/
	ChargerWindUpState::ChargerWindUpState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChargerWindUpState::Enter()
	{
		DebuggerLogWarning("CHARGER WINDUP STATE");
		this->Update();
	}

	void ChargerWindUpState::Update()
	{
		DebuggerLogWarning("CHARGER WINDUP STATE");
		mEnemy->mTimerToCharge -= static_cast<float>(TIME->GetDeltaTime());
		if (mEnemy->mTimerToCharge <= 0.0f)
		{
			GetFSM().ChangeState("Charge");
		}
	}

	void ChargerWindUpState::Exit() { }

	/*!***********************************************************************
	\brief
	Charge state of the Charger enemy
	*************************************************************************/
	ChargerChargeState::ChargerChargeState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChargerChargeState::Enter()
	{
		DebuggerLogWarning("CHARGER CHARGE STATE");
		this->Update();
	}

	void ChargerChargeState::Update()
	{
		DebuggerLogWarning("CHARGER CHARGE STATE");
		//float DistInBwn = Vec2<float>::Distance(mEnemy->GetChargerPos(), mEnemy->GetPlayerPos());

		Vec2<float> Direction = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());
		Vec2<float> NormalForce = Direction * mEnemy->mChargingSpeed;
		mEnemy->GetRigidBody()->addForce(NormalForce * static_cast<float>(TIME->GetDeltaTime())); //add force to move

		

	}

	void ChargerChargeState::Exit()
	{

	}

	/*!***********************************************************************
	\brief
	Stunned state of the Charger enemy
	*************************************************************************/
	ChargerStunnedState::ChargerStunnedState(CPPSCharger* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	void ChargerStunnedState::Enter()
	{
		DebuggerLogWarning("CHARGER STUNNED STATE");
		this->Update();
	}

	void ChargerStunnedState::Update()
	{
		DebuggerLogWarning("CHARGER STUNNED STATE");
		//this->Update();
	}

	void ChargerStunnedState::Exit()
	{

	}
}