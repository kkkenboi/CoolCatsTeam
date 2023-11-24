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

namespace LB {

	/*!***********************************************************************
	\brief
	Start function (Basically initializes everything)
	*************************************************************************/
	void CPPSChaser::Start()
	{
		//Intialise the components
		//GameObj = FACTORY->SpawnGameObject({ C_CPRender, C_CPRigidBody, C_CPCollider });
		if (GameObj->HasComponent<CPRender>()) 
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
		}

		//mRender->UpdateTexture(ASSETMANAGER->GetTextureIndex("chaser"));

		//Then we init all the states
		IdleState* IDLESTATE = DBG_NEW IdleState(this, mFSM, "Idle");
		ChaseState* CHASESTATE = DBG_NEW ChaseState(this, mFSM, "Chase");
		HurtState* HURTSTATE = DBG_NEW HurtState(this, mFSM, "Hurt");

		mFSM.AddState(IDLESTATE);
		mFSM.AddState(CHASESTATE);
		mFSM.AddState(HURTSTATE);

		mFSM.SetCurrentState("Idle");

		std::vector<GameObject*> GOs = GOMANAGER->GetGameObjects();
		for (GameObject* GO : GOs) {
			if (GO->GetName() == "MainChar") {
				mPlayer = GO;
				break;
			}
		}

		mHealth = 3;
		mSpeedMagnitude = 1000.f;

		mInitialised = true;
	}

	/*!***********************************************************************
	\brief
	Update function (updates the FSM)
	*************************************************************************/
	void CPPSChaser::Update()
	{
		//DebuggerLog("In ChaserUpdate\n");
		if (mInitialised == false)
		{
			return;
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

	void CPPSChaser::OnCollisionEnter(CollisionData colData)
	{
		std::cout << "Chaser Colliding with another object" << std::endl;
		if (this->mFSM.GetCurrentState()->GetStateID() == "Chase")
		{
			if (colData.colliderOther->m_gameobj->GetName() == "ball") {
				std::cout << "Got Hit By Ball" << std::endl;
				--mHealth;
				mFSM.ChangeState("Hurt");
			}
		}
	}

	/*!***********************************************************************
	\brief
	Getter for the render component 
	*************************************************************************/
	CPRender* CPPSChaser::GetRender()
	{
		return mRender;
	}

	/*!***********************************************************************
	\brief
	Getter for the rigidbody component 
	*************************************************************************/
	CPRigidBody* CPPSChaser::GetRigidBody()
	{
		return mRigidBody;
	}

	/*!***********************************************************************
	\brief
	Getter for the collider component 
	*************************************************************************/
	CPCollider* CPPSChaser::GetCollider()
	{
		return mCollider;
	}

	/*!***********************************************************************
	\brief
	Getter for the player object 
	*************************************************************************/
	GameObject* CPPSChaser::GetHero()
	{
		return mPlayer;
	}

	/*!***********************************************************************
	\brief
	Getter for speed magnitude
	*************************************************************************/
	float CPPSChaser::GetSpeedMag()
	{
		return mSpeedMagnitude;
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
	void IdleState::Enter()
	{
		this->Update();
	}
	void IdleState::Update()
	{
		if (INPUT->IsKeyPressed(KeyCode::KEY_R)) 
		{
			// Change the state to Chase
			GetFSM().ChangeState("Chase");
		}
	}
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

	void ChaseState::Enter()
	{
		DebuggerLog("Entered ChaseState");
		this->Update();
	}
	void ChaseState::Update()
	{
		// Calculate direction of force from enemy to player
		Vec2<float> CurEnemyPos = mEnemy->GetRigidBody()->getPos();
		Vec2<float> CurHeroPos = mEnemy->GetHero()->GetComponent<CPRigidBody>()->getPos();

		Vec2<float> Direction = CurHeroPos - CurEnemyPos;
		Direction = Normalise(Direction);

		Direction = Direction * mEnemy->GetSpeedMag();
		DebuggerLogFormat("%f, %f", Direction.x, Direction.y);
		mEnemy->GetRigidBody()->addForce(Direction);
	}
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

	void HurtState::Enter()
	{
		this->Update();
	}

	void HurtState::Update()
	{

	}

	void HurtState::Exit()
	{

	}
}