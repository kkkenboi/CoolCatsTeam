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
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Animation/AnimationController.h"

namespace LB
{
	void CPPSCharger::Start()
	{
		CPPSBaseEnemy::Start();
		
		mTransform = GameObj->GetComponent<CPTransform>();

		// Cache the render and animator
		mRender = mTransform->GetChild()->GetChild()->GetComponent<CPRender>();
		mAnimator = mTransform->GetChild()->GetChild()->GetComponent<CPAnimator>();
		mMoveAnim = mTransform->GetChild()->GetComponent<CPAnimator>();
		mDizzyAnim = mTransform->GetChild(2)->GetComponent<CPAnimator>();
		mAngerAnim = mTransform->GetChild(3)->GetComponent<CPAnimator>();
		mAngerTwoAnim = mTransform->GetChild(6)->GetComponent<CPAnimator>();
		//mPuffAnim

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
		mDetectionRange = 800.f;
		/********************************************/
		// Idle State variables
		/********************************************/
		//mIdleCooldown = 4.0f;

		/********************************************/
		// Move State variables
		/********************************************/
		mDistToWindUp = 700.0f;

		/********************************************/
		// Hurt State variables
		/********************************************/
		mTimerDurationHurt = 2.0f;
		m_isHurt = false;

		/********************************************/
		// WindUp State variables
		/********************************************/
		//mTimerToCharge = 3.0f;

		/********************************************/
		// Charge State variables
		/********************************************/
		mChargingSpeed = 2500.f;
		m_isCharging = false;

		/********************************************/
		// Stunned State variables
		/********************************************/
		mTimerWhenStunned = mStunTimerElapsed = 3.0f;
		mInitialised = true;
		m_isStunned = false;

		//********************EFFECTS ON CHARGER***********************
		//DIZZY
		mDizzyObj = mTransform->GetChild(2)->gameObj;
		mDizzyRender = mDizzyObj->GetComponent<CPRender>();

		mDizzyObjTrans = mDizzyObj->GetComponent<CPTransform>();
		mDizzyObjTrans->SetPosition(Vec2<float>(0.f, 80.f));
		mDizzyRender->ToggleActiveFlag(false);

		//ANGER
		mAngerObj = mTransform->GetChild(3)->gameObj;
		mAngerRender = mAngerObj->GetComponent<CPRender>();
		mAngerObjTrans = mAngerObj->GetComponent<CPTransform>();
		mAngerObjTrans->SetPosition(Vec2<float>(0.f, 80.f));
		mAngerRender->ToggleActiveFlag(false);

		mAngerTwoObj = mTransform->GetChild(6)->gameObj; //will be spawnned when got stunned
		mAngerTwoRender = mAngerTwoObj->GetComponent<CPRender>();
		mAngerObjTwoTrans = mAngerTwoObj->GetComponent<CPTransform>();
		mAngerTwoRender->ToggleActiveFlag(false);

		//FootSteps
		FootstepsParticle = mTransform->GetChild(4)->GetComponent<CPParticle>();
		m_FootstepsParticleEmitRate = FootstepsParticle->mEmitterRate;
		FootstepsParticle->mIsActive = false;

		m_isLocked = false;
	}

	void CPPSCharger::Update()
	{
		CPPSBaseEnemy::Update();

		if (!facingLeft)
		{
			mAngerObjTwoTrans->SetPosition(Vec2<float>(58.0f, 7.0f));
			mAngerObjTwoTrans->SetRotation(5.10f);
		}
		else if (facingLeft)
		{
			mAngerObjTwoTrans->SetPosition(Vec2<float>(-58.0f, 7.0f));
			mAngerObjTwoTrans->SetRotation(-5.10f);
		}

		if (mInitialised == false)
		{
			return;
		}
		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		if (m_isCharging == true)
		{
			FootstepsParticle->mIsActive = true;
		}
		else
		{
			FootstepsParticle->mIsActive = false;
		}

		//std::cout << "is stunned : " << m_isStunned << "\n";
		//std::cout << "is charging : " << m_isCharging << "\n";

		if (m_isCharging || m_isStunned || m_isHurt)
		{
			m_isLocked = true;
		}
		else
		{
			m_isLocked = false;
		}

		//if (m_isStunned)
		//{
		//	mFSM.ChangeState("Stunned");
		//}
		//else if (m_isHurt)
		//{
		//	mFSM.ChangeState("Hurt");
		//}

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
		std::string str(colData.colliderOther->m_gameobj->GetName());
		size_t wallStr = str.find("Wall");
		size_t brambleStr = str.find("Bramble");
		size_t MushroomStr = str.find("Mushroom");
		size_t chargerStr = str.find("Charger_Shield");

		if (colData.colliderOther->m_gameobj->GetName() == "ball") 
		{
			//m_isHurt = true;
			mFSM.ChangeState("Hurt");
		}
		else if ((chargerStr!= std::string::npos|| wallStr != std::string::npos || MushroomStr != std::string::npos) && m_isCharging )
		{	
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerHitSounds, 0.2f);
			//m_isStunned = true;
			mFSM.ChangeState("Stunned");
		}
		else if (brambleStr != std::string::npos)
		{
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerHitSounds, 0.2f);
			//m_isHurt = true;
			mFSM.ChangeState("Hurt");
		}
	}

	void CPPSCharger::Hurt()
	{
		isAggro = true;
		if (GetHealth() <= 0)
		{
			AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerDeathSounds, 0.2f);
			Die();
		}
		else
		{
			CPPSBaseEnemy::Hurt();
		}
	}

	void CPPSCharger::Die()
	{
		isChargerDead = true;
		CPPSBaseEnemy::Die();
		//Code to play death anim goes here
	}

	void CPPSCharger::SetShouldFace(bool state)
	{
		mShouldFace = state;
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

	void CPPSCharger::ChangeToStunned()
	{
		mFSM.ChangeState("Stunned");
	}


	//STATES : IDLE, MOVE, HURT, WINDUP, CHARGE, STUNNED
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
		DebuggerLogWarning("CHARGER IDLE STATE");
		mEnemy->SetShouldFace(true);
		//mEnemy->mIdleCooldown = 1.5f;
		this->Update();
	}

	void ChargerIdleState::Update()
	{
		//DebuggerLogWarning("CHARGER IDLE STATE");
		if (mEnemy->GetDistToPlayer() <= mEnemy->mDetectionRange) mEnemy->isAggro = true;
		if (mEnemy->isAggro)
		{
			GetFSM().ChangeState("Move");
		}
	}

	void ChargerIdleState::Exit(){ }

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
		mEnemy->mMoveAnim->m_playSpeed = 1.0f;
		mEnemy->mMoveAnim->PlayRepeat("Action_Move");
		this->Update();
	}

	void ChargerMoveState::Update()
	{
		//charger will walk slowly towards the player
		//when charger is near the player, state will change to windup

		//DebuggerLogWarning("CHARGER MOVE STATE");

		float DistInBwn = Vec2<float>::Distance(mEnemy->GetChargerPos(), mEnemy->GetPlayerPos());

		if (DistInBwn <= mEnemy->mDistToWindUp)
		{
			GetFSM().ChangeState("WindUp");
		}
		else
		{
			Vec2<float> Direction = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());
			Vec2<float> NormalForce = Direction * mEnemy->GetSpeedMag();

			mEnemy->GetRigidBody()->addForce(NormalForce * static_cast<float>(TIME->GetDeltaTime()));
		}
	}

	void ChargerMoveState::Exit() 
	{ 
		mEnemy->mMoveAnim->StopAndReset();
	}

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
		mEnemy->mAnimator->PlayAndReset("Charger_Hurt");
		
		AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerHurtSounds, 0.2f);
		mEnemy->m_isHurt = true;
		mEnemy->Hurt();
		this->Update();
	}

	void ChargerHurtState::Update()
	{
		//DebuggerLogWarning("CHARGER HURT STATE");
		mEnemy->mTimerDurationHurt -= static_cast<float>(TIME->GetDeltaTime());
		if (mEnemy->mTimerDurationHurt <= 0.0f)
		{
			GetFSM().ChangeState("Idle");
		}
	}

	void ChargerHurtState::Exit()
	{
		mEnemy->m_isHurt = false;
		mEnemy->mAnimator->StopAndReset();
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
		mEnemy->mAngerRender->ToggleActiveFlag(true);
		mEnemy->mAngerTwoRender->ToggleActiveFlag(true);
		mEnemy->mAnimator->PlayRepeat("Charger_Prep");
		mEnemy->mAngerAnim->PlayRepeat("Charger_PrepVFX");
		mEnemy->mAngerTwoAnim->PlayRepeat("Charger_PrepVFX2");

		AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerChargingSounds, 0.2f);

		mEnemy->mTimerToCharge = 2.0f;
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

	void ChargerWindUpState::Exit() 
	{
		mEnemy->mAnimator->StopAndReset();
		mEnemy->mAngerAnim->StopAndReset();
		mEnemy->mAngerRender->ToggleActiveFlag(false);
		mEnemy->mAngerTwoRender->ToggleActiveFlag(false);
	}

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
		mEnemy->mAnimator->PlayRepeat("Charger_Charging");
		mEnemy->mMoveAnim->m_playSpeed = 2.0f;
		mEnemy->mMoveAnim->PlayRepeat("Action_Move");
		mEnemy->m_isCharging = true;
		mEnemy->mChargeDirection = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());
		mEnemy->mChargeNormalForce = mEnemy->mChargeDirection * mEnemy->mChargingSpeed;
		AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerAttackSounds, 0.2f);
		mEnemy->SetShouldFace(false);
		
		this->Update();
	}

	void ChargerChargeState::Update()
	{
		mEnemy->GetRigidBody()->mVelocity += mEnemy->mChargeNormalForce  * static_cast<float>(TIME->GetDeltaTime());
		mEnemy->GetRigidBody()->mVelocity.x = Clamp<float>(mEnemy->GetRigidBody()->mVelocity.x, -(mEnemy->mChargingSpeed), mEnemy->mChargingSpeed);
		mEnemy->GetRigidBody()->mVelocity.y = Clamp<float>(mEnemy->GetRigidBody()->mVelocity.y, -(mEnemy->mChargingSpeed), mEnemy->mChargingSpeed);

		
	}

	void ChargerChargeState::Exit()
	{
		mEnemy->m_isCharging = false;
		mEnemy->mAnimator->StopAndReset();
		mEnemy->mMoveAnim->StopAndReset();
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

		mEnemy->mDizzyAnim->PlayRepeat("Charger_StunVFX");
		mEnemy->mAnimator->PlayRepeat("Charger_Stunny");

		mEnemy->m_isStunned = true;
		mEnemy->mStunStopMovingElapsed = 0.0f;

		mEnemy->mStunTimerElapsed = mEnemy->mTimerWhenStunned;
		mEnemy->mDizzyRender->ToggleActiveFlag(true);

		this->Update();
	}

	void ChargerStunnedState::Update()
	{
		//DebuggerLogWarning("CHARGER STUNNED STATE");
		if (mEnemy->mStunStopMovingElapsed < 0.15f)
		{
			mEnemy->mStunStopMovingElapsed += static_cast<float>(TIME->GetDeltaTime());
			if (mEnemy->mStunStopMovingElapsed >= 0.15f)
			{
				mEnemy->GetRigidBody()->mVelocity.x = 0.0f;
				mEnemy->GetRigidBody()->mVelocity.y = 0.0f;
			}
		}

		mEnemy->mStunTimerElapsed -= static_cast<float>(TIME->GetDeltaTime());

		if (mEnemy->mStunTimerElapsed <= 0.0f)
		{
			GetFSM().ChangeState("Idle");
		}
	}

	void ChargerStunnedState::Exit()
	{
		mEnemy->m_isStunned = false;
		mEnemy->mDizzyRender->ToggleActiveFlag(false);
		mEnemy->mDizzyAnim->StopAndReset();
		mEnemy->mAnimator->StopAndReset();
	}
}