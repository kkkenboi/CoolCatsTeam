/*!************************************************************************
 \file				CPPSChaser.cpp
 \author(s)			Justine Carlo Villa Ilao, Vanessa Chua Siew Jin
 \par DP email(s):	justine.c@digipen.edu, vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				03-11-2023
 \brief
 This file contains the CPPSChaser class and all its functionalities,
it handles the logic for the chaser enemy

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSChaser.h"

#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"

#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "CPPGameManager.h"
//#include "CPPVFXManager.h"
#include "CPPAudioManager.h"
namespace LB 
{
	/*!***********************************************************************
	\brief
	Start function (Basically initializes everything)
	*************************************************************************/
	void CPPSChaser::Start()
	{
		CPPSBaseEnemy::Start();
		// Cache the render and animator
		mRender = GetComponent<CPTransform>()->GetChild()->GetChild()->GetComponent<CPRender>();
		mAnimator = GetComponent<CPTransform>()->GetChild()->GetChild()->GetComponent<CPAnimator>();
		m_moveAnimator = GetComponent<CPTransform>()->GetChild()->GetComponent<CPAnimator>();
		m_AudioManager = GOMANAGER->FindGameObjectWithName("AudioManager");
		m_club = GetComponent<CPTransform>()->GetChild()->GetChild()->GetChild()->GetChild()->gameObj;

		//Then we init all the states
		IdleState* IDLESTATE = DBG_NEW IdleState(this, mFSM, "Idle");
		ChaseState* CHASESTATE = DBG_NEW ChaseState(this, mFSM, "Chase");
		HurtState* HURTSTATE = DBG_NEW HurtState(this, mFSM, "Hurt");

		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(HURTSTATE);

		mFSM.SetCurrentState("Idle");

		GetHealth() = 3;
		GetSpeedMag() = 60000.f;
		//since this value is the equivalent of the pixels, 
		//screen is 1920x1080. 800 should be just nice
		mDetectionRange = 800.f;
		mInitialised = true;

	}

	/*!***********************************************************************
	\brief
	FixedUpdate function (Fixedupdates the FSM) run physics
	*************************************************************************/
	void CPPSChaser::FixedUpdate()
	{
		mFSM.FixedUpdate();
	}

	/*!***********************************************************************
	\brief
	Update function (updates the FSM)
	*************************************************************************/
	void CPPSChaser::Update()
	{
		if (mGameManager->GetComponent<CPPSGameManager>()->isGameOver) return;
		CPPSBaseEnemy::Update();
		//Kill command moved to base enemy

		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= static_cast<float>(TIME->GetDeltaTime());
		}

		if (facingLeft != clubFacingLeft)
		{
			if (facingLeft)
			{
				m_club->GetComponent<CPTransform>()->SetPosition({ -20.f, 0.f });
				m_club->GetComponent<CPTransform>()->SetRotation(-120.f);
			}
			else
			{
				m_club->GetComponent<CPTransform>()->SetPosition({ 20.f, 0.f });
				m_club->GetComponent<CPTransform>()->SetRotation(120.f);
			}
			clubFacingLeft = facingLeft;
		}

		mFSM.Update();
	}
	/*!***********************************************************************
	\brief
	Destroy function (will delete the states)
	*************************************************************************/
	void CPPSChaser::Destroy()
	{
		delete mFSM.GetState("Idle");
		delete mFSM.GetState("Chase");
		delete mFSM.GetState("Hurt");
	}

	/*!***********************************************************************
	\brief
	Override Hurt function, play animation
	*************************************************************************/
	void CPPSChaser::Hurt()
	{
		isAggro = true;
		m_moveAnimator->StopAndReset();
		CPPSBaseEnemy::Hurt();
	}

	/*!***********************************************************************
	\brief
	On Collision Enter to check who is it colliding with
	*************************************************************************/
	void CPPSChaser::OnCollisionEnter(CollisionData colData)
	{
		CPPSBaseEnemy::OnCollisionEnter(colData);
		if (colData.colliderOther->m_gameobj->GetName() == "ball") {
			//The knock back value has been edited from 300 -> 800 -> 600
			if (PHY_MATH::Length(colData.colliderOther->GetRigidBody()->mVelocity) > 600.f)
			{
				/*Vec2<float> hitPos = colData.colliderOther->transform->GetPosition();
				GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnHitAnim(hitPos);*/
				DebuggerLogWarningFormat("CHASER HIT! %f", mGotAttackedCooldown);
				if (mGotAttackedCooldown > 0.0f) {
					return;
				}
				DebuggerLogWarning("CHASER HIT ACTUAL!");
				
				mGotAttackedCooldown = mGotAttacked;
				
				mFSM.ChangeState("Hurt");
				Hurt();	//This is here to play the anim
				//We don't move it into the hurt state because hurt state plays when chaser his player
				//We want it to only play hurt when he gets hit by ball
				
			}
		}
		if (colData.colliderOther->m_gameobj->GetName() == "Bramble")
		{
			if (mGotAttackedCooldown > 0.0f) {
				return;
			}
			mGotAttackedCooldown = mGotAttacked;
			mFSM.ChangeState("Hurt");
			Hurt();	//This is here to play the anim
		}
		if (colData.colliderOther->m_gameobj->GetName() == "MainChar") 
		{ 
			GetAnimator()->PlayAndReset("Melee_Attack");
			//if (facingLeft)
			//{
			//	m_clubAnim->PlayAndReset("Melee_Swing_Left");
			//}
			//else
			//{
			//	m_clubAnim->PlayAndReset("Melee_Swing_Right");
			//}
			m_moveAnimator->StopAndReset();
			mFSM.ChangeState("Idle"); 
		}

	}

	void CPPSChaser::OnCollisionStay(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "Sandpit")
		{
			m_moveAnimator->m_playSpeed = 0.6f;
		}
	}

	void CPPSChaser::OnCollisionExit(CollisionData colData)
	{
		if (colData.colliderOther->gameObj->GetName() == "Sandpit")
		{
			m_moveAnimator->m_playSpeed = 1.0f;
		}
	}

	/*!***********************************************************************
	\brief
	Override Die function from base enemy, play sound
	*************************************************************************/
	void CPPSChaser::Die()
	{
		//We access the base class first
		//GOMANAGER->FindGameObjectWithName("VFXManager")->GetComponent<CPPSVFXManager>()->SpawnPoofAnim(GetComponent<CPTransform>()->GetPosition());
		m_AudioManager->GetComponent<CPPSAudioManager>()->Play3DSound(m_AudioManager->GetComponent<CPPSAudioManager>()->ChaserDeathSounds, GetComponent<CPTransform>()->GetPosition(), false, 0.2f);
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChaserDeathSounds,0.2f);
		CPPSBaseEnemy::Die(); //We call this because the base class will have some logic to reduce enemy count
		//Code to play death anim goes here
	}

	/*!***********************************************************************
	\brief
	Getter for hurt timer
	*************************************************************************/
	float& CPPSChaser::GetHurtTimer()
	{
		return mHurtTimer;
	}

	// States ===================

	// IDLE STATE FUNCTIONS !!!

	/*!***********************************************************************
	\brief
	IDLE STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	IdleState::IdleState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	/*!***********************************************************************
	\brief
	Entering of idle state
	*************************************************************************/
	void IdleState::Enter()
	{
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate of idle state
	*************************************************************************/
	void IdleState::FixedUpdate(){}

	/*!***********************************************************************
	\brief
	Update of idle state
	*************************************************************************/
	void IdleState::Update()
	{
		//I can either do this check in the normal update, or over here.
		if (mEnemy->GetDistToPlayer() <= mEnemy->mDetectionRange) mEnemy->isAggro = true;
		if (mEnemy->isAggro)
		{
			GetFSM().ChangeState("Chase");
			GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>()->Play3DSound(GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>()->ChaserAttackSounds, mEnemy->GameObj->GetComponent<CPTransform>()->GetPosition(), false, 0.2f);

			//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChaserAttackSounds,0.2f);
		}
	}

	/*!***********************************************************************
	\brief
	Exit of the Idle State
	*************************************************************************/
	void IdleState::Exit(){}

	// CHASE STATE FUNCTIONS !!!
	/*!***********************************************************************
	\brief
	CHASE STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	ChaseState::ChaseState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	/*!***********************************************************************
	\brief
	Entering of chase state
	*************************************************************************/
	void ChaseState::Enter()
	{
		//DebuggerLog("Entered ChaseState");
		mEnemy->m_moveAnimator->PlayRepeat("Action_Move");
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate of chase state
	*************************************************************************/
	void ChaseState::FixedUpdate()
	{
		// Calculate direction of force from enemy to player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos();
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos();

		Vec2<float> Direction = (CurHeroPos - CurEnemyPos).Normalise();
		Vec2<float> NormalForce = Direction * mEnemy->GetSpeedMag();

		mEnemy->GetRigidBody()->addForce(NormalForce * static_cast<float>(TIME->GetFixedDeltaTime()));
	}

	/*!***********************************************************************
	\brief
	Update of chase state where it gets the position of itself and the player so that
	the chaser will go towards the player
	*************************************************************************/
	void ChaseState::Update(){}

	/*!***********************************************************************
	\brief
	Exiting of exit state
	*************************************************************************/
	void ChaseState::Exit(){}

	// HURT STATE FUNCTIONS !!!
	/*!***********************************************************************
	\brief
	HURT STATE FUNCTIONS (Ctor, Enter,Update,Exit)
	*************************************************************************/
	HurtState::HurtState(CPPSChaser* enemy_ptr, FiniteStateMachine& fsm, std::string name) :
		State(fsm, name)
	{
		mEnemy = enemy_ptr;
	}

	/*!***********************************************************************
	\brief
	Entering of Hurt state, initialising the values
	*************************************************************************/
	void HurtState::Enter()
	{
		//DebuggerLog("Entered HurtState");
		mEnemy->GetHurtTimer() = 1.5f;
		mEnemy->mAnimator->PlayAndReset("Melee_Hurt");
		GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>()->Play3DSound(GOMANAGER->FindGameObjectWithName("AudioManager")->GetComponent<CPPSAudioManager>()->ChaserHurtSounds, mEnemy->GameObj->GetComponent<CPTransform>()->GetPosition(), false, 0.2f);

		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChaserHurtSounds,0.2f);
		this->Update();
	}

	/*!***********************************************************************
	\brief
	FixedUpdate of hurt state
	*************************************************************************/
	void HurtState::FixedUpdate(){}

	/*!***********************************************************************
	\brief
	Updating of hurt state
	*************************************************************************/
	void HurtState::Update()
	{
		mEnemy->GetHurtTimer() -= static_cast<float>(TIME->GetDeltaTime());
		if (mEnemy->GetHurtTimer() <= 0.f) 
		{
			this->GetFSM().ChangeState("Chase");
		}
	}

	/*!***********************************************************************
	\brief
	Exit of Hurt state
	*************************************************************************/
	void HurtState::Exit()
	{
		mEnemy->GetHurtTimer() = 1.5f;
	}
}