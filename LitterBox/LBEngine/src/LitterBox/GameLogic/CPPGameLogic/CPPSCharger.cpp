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
//#include "CPPShield.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Serialization/AssetManager.h"

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
		GetHealth() = 3;
		GetSpeedMag() = 50000.f; //speed of movement

		isChargerDead = false;

		/********************************************/
		// Idle State variables
		/********************************************/
		//mIdleCooldown = 4.0f;

		/********************************************/
		// Move State variables
		/********************************************/
		mDistToWindUp = 700.0f;

		/********************************************/
		// WindUp State variables
		/********************************************/
		//mTimerToCharge = 3.0f;

		/********************************************/
		// Charge State variables
		/********************************************/
		mChargingSpeed = 150000.f;
		mIsCharging = false;

		/********************************************/
		// Stunned State variables
		/********************************************/
		mTimerWhenStunned = 3.0f;

		mInitialised = true;

		//mAttackCooldown = 2.0f;
		//mAttackCooldownCurrent = 0.0f;
	}

	void CPPSCharger::Update()
	{
		CPPSBaseEnemy::Update();
		if (mInitialised == false)
		{
			return;
		}
		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
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

		// Destroy the shield gameobject
		//CPTransform* transform = GetComponent<CPTransform>();
		//if (transform->GetChildCount())
		//{
		//	transform->GetChild()->Destroy();
		//}
	}

	void CPPSCharger::OnCollisionEnter(CollisionData colData)
	{
		CPPSBaseEnemy::OnCollisionEnter(colData);
		if (colData.colliderOther->m_gameobj->GetName() == "ball") 
		{
			if (GetHealth() <= 0)
			{
				Die();
			}
			mFSM.ChangeState("Hurt");
		}
		else if (mIsCharging == true)
		{
			mFSM.ChangeState("Stunned");
		}
	}

	void CPPSCharger::Hurt()
	{
		CPPSBaseEnemy::Hurt();
	}

	void CPPSCharger::Die()
	{
		isChargerDead = true;
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
		//DebuggerLogWarning("CHARGER IDLE STATE");
		mEnemy->mIdleCooldown = 1.5f;
		this->Update();
	}

	void ChargerIdleState::Update()
	{
		//this->Update();
		//NEED TO CALCULATE THE DISTANCE BWN THE CHARGER AND THE PLAYER BECOS THE MAP IS GG TO BE BIGGER!!!!
		//DebuggerLogWarning("CHARGER IDLE STATE");
		mEnemy->mIdleCooldown -= static_cast<float>(TIME->GetDeltaTime());
		//DebuggerLogErrorFormat("Timer Idle: %i", mEnemy->mIdleCooldown);
		if (mEnemy->mIdleCooldown <= 0.0f) //4 secs
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
		//DebuggerLogWarning("CHARGER MOVE STATE");
		this->Update();

	}

	void ChargerMoveState::Update()
	{
		//charger will walk slowly towards the player
		//when charger is near the player, state will change to windup
		float DistInBwn = Vec2<float>::Distance(mEnemy->GetChargerPos(), mEnemy->GetPlayerPos());
		
		//DebuggerLogWarning("CHARGER MOVE STATE");
		//DebuggerLogFormat("Dist In Between enemy and Hero: %f", DistInBwn);

		Vec2<float> Direction = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());

		Vec2<float> NormalForce = Direction * mEnemy->GetSpeedMag();

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
		//DebuggerLogWarning("CHARGER HURT STATE");
		mEnemy->Hurt();
		this->Update();
	}

	void ChargerHurtState::Update()
	{
		//DebuggerLogWarning("CHARGER HURT STATE");
		GetFSM().ChangeState("Idle");
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
		//DebuggerLogWarning("CHARGER WINDUP STATE");
		mEnemy->mTimerToCharge = 1.0f;
		this->Update();
	}

	void ChargerWindUpState::Update()
	{
		//DebuggerLogWarning("CHARGER WINDUP STATE");
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
		//DebuggerLogWarning("CHARGER CHARGE STATE");
		mEnemy->mChargeDirection = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());
		mEnemy->mChargeNormalForce = mEnemy->mChargeDirection * mEnemy->mChargingSpeed;
		mEnemy->mIsCharging = true;
		AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerAttackSounds, 0.2f);
		this->Update();
	}

	void ChargerChargeState::Update()
	{
		//Now when charge, it suppose to calculate the direction once and thats it
		//DebuggerLogWarning("CHARGER CHARGE STATE");
		mEnemy->GetRigidBody()->addForce(mEnemy->mChargeNormalForce * static_cast<float>(TIME->GetDeltaTime())); //add force to move
		//DebuggerLogErrorFormat("Is Charging bool: %i", mEnemy->mIsCharging);
		//After it collides with any other game object
		//Check on collision
		//it will change it state back to idle
		//rinse and repeat

		//if (mEnemy->mIsCharging == true)
		//{
		//	GetFSM().ChangeState("Stunned");
		//}

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
		//DebuggerLogWarning("CHARGER STUNNED STATE");
		mEnemy->mIsCharging = false;
		this->Update();
	}

	void ChargerStunnedState::Update()
	{
		//DebuggerLogWarning("CHARGER STUNNED STATE");
		//mEnemy->mIsCharging = false;
		//DebuggerLogErrorFormat("Is Charging bool: %i", mEnemy->mIsCharging);
		mEnemy->mTimerWhenStunned -= static_cast<float>(TIME->GetDeltaTime());
		if (mEnemy->mTimerWhenStunned <= 0.0f)
		{
			GetFSM().ChangeState("Idle");
		}
	}

	void ChargerStunnedState::Exit()
	{

	}



}