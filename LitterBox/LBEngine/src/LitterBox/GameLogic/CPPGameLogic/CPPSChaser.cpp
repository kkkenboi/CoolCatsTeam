/*!************************************************************************
 \file				CPPSChaser.cpp
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

#include "CPPSChaser.h"

#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Debugging/Debug.h"

#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Physics/PhysicsMath.h"

namespace LB 
{
	static int count{};
	/*!***********************************************************************
	\brief
	Start function (Basically initializes everything)
	*************************************************************************/
	void CPPSChaser::Start()
	{
		CPPSBaseEnemy::Start();
		//Intialise the components
		// 		right_face = trans->GetScale();
	/*	rightFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace = GameObj->GetComponent<CPTransform>()->GetScale();
		leftFace.x = -leftFace.x;*/
		//GameObj = FACTORY->SpawnGameObject({ C_CPRender, C_CPRigidBody, C_CPCollider });
		/*if (GameObj->HasComponent<CPRender>()) 
		{
			mRender = GameObj->GetComponent<CPRender>();
		}
		else
		{
			mRender = nullptr;
			return;
		}
		if (GameObj->HasComponent<CPRigidBody>())
		{
			mRigidBody = GameObj->GetComponent<CPRigidBody>();
		}
		else 
		{
			mRigidBody = nullptr;
			return;
		}
		if (GameObj->HasComponent<CPCollider>())
		{
			mCollider = GameObj->GetComponent<CPCollider>();
		}
		else 
		{
			mCollider = nullptr;
			return;
		}*/

		//mRender->UpdateTexture(ASSETMANAGER->GetTextureIndex("chaser"));

		//Then we init all the states
		IdleState* IDLESTATE = DBG_NEW IdleState(this, mFSM, "Idle");
		ChaseState* CHASESTATE = DBG_NEW ChaseState(this, mFSM, "Chase");
		HurtState* HURTSTATE = DBG_NEW HurtState(this, mFSM, "Hurt");

		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(HURTSTATE);

		mFSM.SetCurrentState("Idle");

	/*	std::vector<GameObject*> GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}*/

		mGotAttacked = 0.5f;
		mGotAttackedCooldown = 0.0f;

		mHealth = 3;
		mSpeedMagnitude = 100000.f;

		mInitialised = true;
	}

	/*!***********************************************************************
	\brief
	Update function (updates the FSM)
	*************************************************************************/
	void CPPSChaser::Update()
	{
		CPPSBaseEnemy::Update();
		//DebuggerLog("In ChaserUpdate\n");
		//Kill command moved to base enemy
	/*	if (INPUT->IsKeyPressed(KeyCode::KEY_0))
		{
			mShouldDestroy = true;
		}*/
		if (mShouldDestroy)
		{
			GOMANAGER->RemoveGameObject(this->GameObj);
			return;
		}
		if (mGotAttackedCooldown > 0.0f) {
			mGotAttackedCooldown -= static_cast<float>(TIME->GetDeltaTime());
		}
	/*	Vec2<float> DirToPlayer = mPlayer->GetComponent<CPTransform>()->GetPosition() - GameObj->GetComponent<CPTransform>()->GetPosition();
		Vec2<float> TransformRight{ 1,0 };
		if (DotProduct(DirToPlayer.Normalise(), TransformRight) < 0.0f)
		{
			GameObj->GetComponent<CPTransform>()->SetScale(leftFace);
		} else GameObj->GetComponent<CPTransform>()->SetScale(rightFace);*/
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

	void CPPSChaser::Hurt()
	{
		GetComponent<CPAnimator>()->Play("MeleeHurt");
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

			if (PHY_MATH::Length(colData.colliderOther->GetRigidBody()->mVelocity) > 200.f)
			{
				DebuggerLogWarningFormat("CHASER HIT! %f", mGotAttackedCooldown);
				if (mGotAttackedCooldown > 0.0f) {
					return;
				}
				DebuggerLogWarning("CHASER HIT ACTUAL!");

				//int Channel = AUDIOMANAGER->PlaySound("Enemy hurt");
				//AUDIOMANAGER->SetChannelVolume(Channel, 0.7f);
				
				mGotAttackedCooldown = mGotAttacked;

				
				mFSM.ChangeState("Hurt");
				Hurt();
				
			}
		}
		if (colData.colliderOther->m_gameobj->GetName() == "MainChar") { 
			GetComponent<CPAnimator>()->Play("MeleeAttack");
			mFSM.ChangeState("Hurt"); 
		}

	}

	void CPPSChaser::Die()
	{
		//We access the base class first
		AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChaserDeathSounds,0.2f);
		CPPSBaseEnemy::Die();
	/*	GameObj->GetComponent<CPTransform>()->SetPosition(Vec2<float>{0.0f, 10000.0f});
		GameObj->RemoveComponent(C_CPCollider);
		mShouldDestroy = true;*/
		//Code to play death anim goes here
	}

	/*!***********************************************************************
	\brief
	Getter for the render component 
	*************************************************************************/
	/*CPRender* CPPSChaser::GetRender()
	{
		return mRender;
	}*/

	/*!***********************************************************************
	\brief
	Getter for the rigidbody component 
	*************************************************************************/
	/*CPRigidBody* CPPSChaser::GetRigidBody()
	{
		return mRigidBody;
	}*/

	/*!***********************************************************************
	\brief
	Getter for the collider component 
	*************************************************************************/
	/*CPCollider* CPPSChaser::GetCollider()
	{
		return mCollider;
	}*/

	/*!***********************************************************************
	\brief
	Getter for the player object 
	*************************************************************************/
	/*GameObject* CPPSChaser::GetHero()
	{
		return mPlayer;
	}*/

	/*!***********************************************************************
	\brief
	Getter for player's health
	*************************************************************************/
	/*int& CPPSChaser::GetHealth()
	{
		return mHealth;
	}*/

	/*!***********************************************************************
	\brief
	Getter for speed magnitude
	*************************************************************************/
	/*float& CPPSChaser::GetSpeedMag()
	{
		return mSpeedMagnitude;
	}*/

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
	Update of idle state
	*************************************************************************/
	void IdleState::Update()
	{
		GetFSM().ChangeState("Chase");
		//if (INPUT->IsKeyPressed(KeyCode::KEY_R)) 
		//{
		//	// Change the state to Chase
		//}
	}

	/*!***********************************************************************
	\brief
	Exit of the Idle State
	*************************************************************************/
	void IdleState::Exit()
	{
	}

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
		DebuggerLog("Entered ChaseState");
		//AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChaserAttackSounds);
		this->Update();
	}

	/*!***********************************************************************
	\brief
	Update of chase state where it gets the position of itself and the player so that
	the chaser will go towards the player
	*************************************************************************/
	void ChaseState::Update()
	{
		// Calculate direction of force from enemy to player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos();
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos();

		Vec2<float> Direction = CurHeroPos - CurEnemyPos;
		Direction = Normalise(Direction);

		Direction = Direction * mEnemy->GetSpeedMag();
		//DebuggerLogFormat("%f, %f", Direction.x, Direction.y);
		mEnemy->GetRigidBody()->addForce(Direction * TIME->GetDeltaTime());
	}

	/*!***********************************************************************
	\brief
	Exiting of exit state
	*************************************************************************/
	void ChaseState::Exit()
	{
	}



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
		//std::cout << "Health: " << mEnemy->GetHealth();
		mEnemy->GetHurtTimer() = 1.5f;
		AUDIOMANAGER->PlayRandomisedSound(AUDIOMANAGER->ChaserHurtSounds,0.2f);
		this->Update();
	}

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

		if (mEnemy->GetHealth() <= 0) 
		{
			//mEnemy->GetCollider()
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