/*!************************************************************************
 \file				CPPSCharger.cpp
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

#include "CPPSCharger.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Animation/AnimationController.h"
#include "CPPAudioManager.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Starting behaviour for Charger where variables are initialized
	*************************************************************************/
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
		mAudioManager = GOMANAGER->FindGameObjectWithName("AudioManager");
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
		GetHealth() = 4;
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

	/*!***********************************************************************
	\brief
	Update behaviour for Charger, runs every check every frame
	*************************************************************************/
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
		if (m_isCharging == true)
		{
			FootstepsParticle->mIsActive = true;
		}
		else
		{
			FootstepsParticle->mIsActive = false;
		}

		if (m_isCharging || m_isStunned || m_isHurt)
		{
			m_isLocked = true;
		}
		else
		{
			m_isLocked = false;
		}
		mFSM.Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate Behaviour for Charger for any physics calculations
	*************************************************************************/

	void CPPSCharger::FixedUpdate()
	{
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
		mFSM.FixedUpdate();
	}

	/*!***********************************************************************
	\brief
	Destroy function for Charger, clear the states
	*************************************************************************/
	void CPPSCharger::Destroy()
	{
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Move");
		delete mFSM.GetState("Hurt");
		delete mFSM.GetState("WindUp");
		delete mFSM.GetState("Charge");
		delete mFSM.GetState("Stunned");
	}

	/*!***********************************************************************
	\brief
	On collision to check who it is colliding with and what will happen
	*************************************************************************/
	void CPPSCharger::OnCollisionEnter(CollisionData colData)
	{
		CPPSBaseEnemy::OnCollisionEnter(colData);
		std::string str(colData.colliderOther->m_gameobj->GetName());
		size_t wallStr = str.find("Wall");
		size_t brambleStr = str.find("Bramble");
		size_t MushroomStr = str.find("Mushroom");
		size_t chargerStr = str.find("Charger_Shield");
		size_t rockStr = str.find("Rock");

		if (colData.colliderOther->m_gameobj->GetName() == "ball") //if ball
		{
			//m_isHurt = true;
			mFSM.ChangeState("Hurt");//change state
		}
		else if ((chargerStr!= std::string::npos|| wallStr != std::string::npos || MushroomStr != std::string::npos || rockStr != std::string::npos /*|| brambleStr != std::string::npos*/ ) && m_isCharging ) //if colliding and is charging
		{	
			//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerHitSounds, 0.2f);
			mAudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(mAudioManager->GetComponent<CPPSAudioManager>()->ChargerHitSound, mTransform->GetPosition(), false, 0.2f, 1.0f);
			//m_isStunned = true;
			mFSM.ChangeState("Stunned"); //change state
		}
		else if (brambleStr != std::string::npos) //if colliding 
		{
			//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerHitSounds, 0.2f);
			mAudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(mAudioManager->GetComponent<CPPSAudioManager>()->ChargerHitSound, mTransform->GetPosition(), false, 0.2f, 1.0f);

			//m_isHurt = true;
			mFSM.ChangeState("Hurt"); //change state
		}
	}

	/*!***********************************************************************
	\brief
	Hurt function for Charger, Call base enemy hurt function, aggro to true
	*************************************************************************/
	void CPPSCharger::Hurt()
	{
		//isAggro = true;
		//CPPSBaseEnemy::Hurt();
		if (GetHealth() <= 0) //if health below 0, die
		{

			//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerDeathSounds, 0.2f);
			mAudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(
				mAudioManager->GetComponent<CPPSAudioManager>()->ChargerDeathSounds,
				mTransform->GetPosition(),false, 0.2f);
			Die();
		}
		else
		{
			isAggro = true; //get hurt, got angry
			CPPSBaseEnemy::Hurt(); //just get hurt
		}
	}

	/*!***********************************************************************
	\brief
	Die function call for Charger
	*************************************************************************/
	void CPPSCharger::Die()
	{
		isChargerDead = true;
		CPPSBaseEnemy::Die();
		//Code to play death anim goes here
	}

	/*!***********************************************************************
	\brief
	Check face side
	*************************************************************************/
	void CPPSCharger::SetShouldFace(bool state)
	{
		mShouldFace = state;
	}

	/*!***********************************************************************
	\brief
	Getter for player pos
	*************************************************************************/
	Vec2<float> CPPSCharger::GetPlayerPos() //get curr player pos
	{
		return GetHero()->GetComponent<CPRigidBody>()->getPos();
	}

	/*!***********************************************************************
	\brief
	Getter for Charger pos
	*************************************************************************/
	Vec2<float> CPPSCharger::GetChargerPos()
	{
		return GetRigidBody()->getPos();
	}

	/*!***********************************************************************
	\brief
	calculate direction
	*************************************************************************/
	Vec2<float> CPPSCharger::DirBToA(Vec2<float> a, Vec2<float> b)
	{
		return (a - b).Normalise();
	}

	/*!***********************************************************************
	\brief
	function to change state for the shield
	*************************************************************************/
	void CPPSCharger::ChangeToStunned()
	{
		mFSM.ChangeState("Stunned");
	}

	/*!***********************************************************************
	\brief
	function to change state for the shield
	*************************************************************************/
	void CPPSCharger::ChangeToHurt()
	{
		mFSM.ChangeState("Hurt");
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


	/*!***********************************************************************
	\brief
	Enter the state of Idle where it will initialise the values
	*************************************************************************/
	void ChargerIdleState::Enter()
	{
		DebuggerLogWarning("CHARGER IDLE STATE");
		mEnemy->SetShouldFace(true);
		//mEnemy->mIdleCooldown = 1.5f;
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of Idle 
	*************************************************************************/
	void ChargerIdleState::FixedUpdate()
	{
	}

	/*!***********************************************************************
	\brief
	Update the state of Idle 
	*************************************************************************/
	void ChargerIdleState::Update()
	{
		//DebuggerLogWarning("CHARGER IDLE STATE");
		if (mEnemy->GetDistToPlayer() <= mEnemy->mDetectionRange) mEnemy->isAggro = true;
		if (mEnemy->isAggro)
		{
			GetFSM().ChangeState("Move");
		}
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of Move where it will initialise the values
	*************************************************************************/
	void ChargerMoveState::Enter()
	{
		DebuggerLogWarning("CHARGER MOVE STATE");
		mEnemy->mMoveAnim->m_playSpeed = 1.0f;
		mEnemy->mMoveAnim->PlayRepeat("Action_Move");
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of Move
	*************************************************************************/
	void ChargerMoveState::FixedUpdate()
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

			mEnemy->GetRigidBody()->addForce(NormalForce * static_cast<float>(TIME->GetFixedDeltaTime()));
		}
	}

	/*!***********************************************************************
	\brief
	Update the state of Move 
	*************************************************************************/
	void ChargerMoveState::Update()
	{
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of Hurt where it will initialise the values
	*************************************************************************/
	void ChargerHurtState::Enter()
	{
		DebuggerLogWarning("CHARGER HURT STATE");
		mEnemy->mAnimator->PlayAndReset("Charger_Hurt");
		
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerHurtSounds, 0.2f);
		mEnemy->mAudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(mEnemy->mAudioManager->GetComponent<CPPSAudioManager>()->ChargerHurtSounds, mEnemy->mTransform->GetPosition(), false, 0.2f, 1.0f);

		mEnemy->m_isHurt = true;
		mEnemy->Hurt();
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of Hurt
	*************************************************************************/
	void ChargerHurtState::FixedUpdate()
	{
	}

	/*!***********************************************************************
	\brief
	Update the state of Hurt
	*************************************************************************/
	void ChargerHurtState::Update()
	{
		//DebuggerLogWarning("CHARGER HURT STATE");
		mEnemy->mTimerDurationHurt -= static_cast<float>(TIME->GetDeltaTime());
		//std::cout << "Time to Idle: " << mEnemy->mTimerDurationHurt << "\n";
		if (mEnemy->mTimerDurationHurt <= 0.0f)
		{
			//std::cout << "CHANGING TO IDLE\n";
			GetFSM().ChangeState("Idle");
		}
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
	void ChargerHurtState::Exit()
	{
		mEnemy->m_isHurt = false;
		mEnemy->mTimerDurationHurt = 2.0f;
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

	/*!***********************************************************************
	\brief
	Enter the state of WindUp where it will initialise the values
	*************************************************************************/
	void ChargerWindUpState::Enter()
	{
		DebuggerLogWarning("CHARGER WINDUP STATE");
		mEnemy->mAngerRender->ToggleActiveFlag(true);
		mEnemy->mAngerTwoRender->ToggleActiveFlag(true);
		mEnemy->mAnimator->PlayRepeat("Charger_Prep");
		mEnemy->mAngerAnim->PlayRepeat("Charger_PrepVFX");
		mEnemy->mAngerTwoAnim->PlayRepeat("Charger_PrepVFX2");

		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerChargingSounds, 0.2f);
		mEnemy->mAudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(mEnemy->mAudioManager->GetComponent<CPPSAudioManager>()->ChargerChargeSound, mEnemy->mTransform->GetPosition(), false, 0.2f, 1.0f);

		mEnemy->mTimerToCharge = 2.0f;

		//mEnemy->isAggro = false;
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate the state of WindUp
	*************************************************************************/
	void ChargerWindUpState::FixedUpdate()
	{
	}

	/*!***********************************************************************
	\brief
	Update the state of WindUp
	*************************************************************************/
	void ChargerWindUpState::Update()
	{
		//DebuggerLogWarning("CHARGER WINDUP STATE");
		mEnemy->mTimerToCharge -= static_cast<float>(TIME->GetDeltaTime());
		if (mEnemy->mTimerToCharge <= 0.0f)
		{
			GetFSM().ChangeState("Charge");
		}
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	Enter the state of Charge where it will initialise the values
	*************************************************************************/
	void ChargerChargeState::Enter()
	{
		DebuggerLogWarning("CHARGER CHARGE STATE");
		mEnemy->mAnimator->PlayRepeat("Charger_Charging");
		mEnemy->mMoveAnim->m_playSpeed = 2.0f;
		mEnemy->mMoveAnim->PlayRepeat("Action_Move");
		mEnemy->m_isCharging = true;
		mEnemy->mChargeDirection = mEnemy->DirBToA(mEnemy->GetPlayerPos(), mEnemy->GetChargerPos());
		mEnemy->mChargeNormalForce = mEnemy->mChargeDirection * mEnemy->mChargingSpeed;
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChargerAttackSounds, 0.2f);
		mEnemy->mAudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(mEnemy->mAudioManager->GetComponent<CPPSAudioManager>()->ChargerAttackSounds, mEnemy->mTransform->GetPosition(), false, 0.2f, 1.0f);

		mEnemy->SetShouldFace(false);
		
		this->Update();
	}

	/*!***********************************************************************
   \brief
   FixedUpdate the state of Charge
   *************************************************************************/
	void ChargerChargeState::FixedUpdate()
	{
		mEnemy->GetRigidBody()->mVelocity += mEnemy->mChargeNormalForce * static_cast<float>(TIME->GetFixedDeltaTime());
		mEnemy->GetRigidBody()->mVelocity.x = Clamp<float>(mEnemy->GetRigidBody()->mVelocity.x, -(mEnemy->mChargingSpeed), mEnemy->mChargingSpeed);
		mEnemy->GetRigidBody()->mVelocity.y = Clamp<float>(mEnemy->GetRigidBody()->mVelocity.y, -(mEnemy->mChargingSpeed), mEnemy->mChargingSpeed);
	}

	/*!***********************************************************************
   \brief
   Update the state of Charge
   *************************************************************************/
	void ChargerChargeState::Update()
	{
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
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

	/*!***********************************************************************
   \brief
   Enter the state of Stunned where it will initialise the values
   *************************************************************************/
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

	/*!***********************************************************************
	\brief
	Update the state of Stunned
	*************************************************************************/
	void ChargerStunnedState::FixedUpdate()
	{
		if (mEnemy->mStunStopMovingElapsed < 0.15f)
		{
			mEnemy->mStunStopMovingElapsed += static_cast<float>(TIME->GetFixedDeltaTime());
			if (mEnemy->mStunStopMovingElapsed >= 0.15f)
			{
				mEnemy->GetRigidBody()->mVelocity.x = 0.0f;
				mEnemy->GetRigidBody()->mVelocity.y = 0.0f;
			}
		}

		//mEnemy->mStunTimerElapsed -= static_cast<float>(TIME->GetFixedDeltaTime());

		//if (mEnemy->mStunTimerElapsed <= 0.0f)
		//{
		//	GetFSM().ChangeState("Idle");
		//}
	}

	/*!***********************************************************************
	\brief
	Update the state of Stunned
	*************************************************************************/
	void ChargerStunnedState::Update()
	{
		//DebuggerLogWarning("CHARGER STUNNED STATE");
		//if (mEnemy->mStunStopMovingElapsed < 0.15f)
		//{
		//	mEnemy->mStunStopMovingElapsed += static_cast<float>(TIME->GetDeltaTime());
		//	if (mEnemy->mStunStopMovingElapsed >= 0.15f)
		//	{
		//		mEnemy->GetRigidBody()->mVelocity.x = 0.0f;
		//		mEnemy->GetRigidBody()->mVelocity.y = 0.0f;
		//	}
		//}

		mEnemy->mStunTimerElapsed -= static_cast<float>(TIME->GetDeltaTime());

		if (mEnemy->mStunTimerElapsed <= 0.0f)
		{
			GetFSM().ChangeState("Idle");
		}
	}

	/*!***********************************************************************
	\brief
	Exit the state
	*************************************************************************/
	void ChargerStunnedState::Exit()
	{
		mEnemy->m_isStunned = false;
		//mEnemy->WallImpactParticle->mIsActive = false;
		mEnemy->mDizzyRender->ToggleActiveFlag(false);
		mEnemy->mDizzyAnim->StopAndReset();
		mEnemy->mAnimator->StopAndReset();
	}
}